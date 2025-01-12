/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "converterhelper.h"
#include "qt-version-check.h"

#include <QStringList>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QRegExp>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QTextStream>
#include <QStringBuilder>
#include <QJSEngine>
#include <QQmlEngine>

#include "bitstream.h"
#include "bitmaphelper.h"
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "reorderingoptions.h"
#include "imageoptions.h"
#include "rlecompressor.h"
#include "convimagepixels.h"
#include "convimagescan.h"

namespace Parsing
{
namespace Conversion
{

void ConverterHelper::pixelsData(
  Settings::Presets::PrepareOptions *prepare,
  const QString &scanScript,
  const QString &pixelScript,
  const QImage *image,
  QVector<quint32> *data,
  int *width, int *height)
{
  if (image != nullptr && data != nullptr && width != nullptr && height != nullptr) {
    data->clear();

    QImage im = *image;

    *width = im.width();
    *height = im.height();

    // monochrome image needs special preprocessing
    Settings::Presets::ConversionType type = prepare->convType();

    if (type == Parsing::Conversion::Options::ConversionType::Monochrome) {
      Settings::Presets::MonochromeType monotype = prepare->monoType();
      int edge = prepare->edge();

      switch (monotype) {
        case Parsing::Conversion::Options::MonochromeType::Edge:
          ConverterHelper::makeMonochrome(im, edge);
          break;

        case Parsing::Conversion::Options::MonochromeType::DiffuseDither:
          im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
          im = im.convertToFormat(QImage::Format_ARGB32);
          break;

        case Parsing::Conversion::Options::MonochromeType::OrderedDither:
          im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
          im = im.convertToFormat(QImage::Format_ARGB32);
          break;

        case Parsing::Conversion::Options::MonochromeType::ThresholdDither:
          im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
          im = im.convertToFormat(QImage::Format_ARGB32);
          break;
      }
    } else if (type == Parsing::Conversion::Options::ConversionType::Grayscale) {
      ConverterHelper::makeGrayscale(im);
    }

    {
      ConvImageScan *convImage = new ConvImageScan(&im);
      convImage->setBandSize(prepare->bandWidth());
      convImage->setUseBands(prepare->bandScanning());

      QString errorMessage;
      ConverterHelper::collectPoints(convImage, scanScript, &errorMessage);

      if (convImage->pointsCount() > 2) {
        ConverterHelper::getImageDataSize(prepare, convImage, width, height);

        // get pixels color
        for (int i = 0; i < convImage->pointsCount(); i++) {
          QPoint point = convImage->pointAt(i);

          // point in image rectangle
          if (point.x() >= 0 && point.y() >= 0 && point.x() < im.width() && point.y() < im.height()) {
            QRgb pixel = im.pixel(point.x(), point.y());
            quint32 value = (quint32)pixel;
            data->append(value);
          } else { // out of image rectangle (bands)
            data->append(0x00000000);
          }
        }
      }

      delete convImage;
    }

    if (type == Parsing::Conversion::Options::ConversionType::Custom) {
      QString errorMessage;
      ConverterHelper::convertPixelsByScript(pixelScript, data, &errorMessage);
    }
  }
}

void ConverterHelper::collectPoints(ConvImageScan *convImage, const QString &script, QString *resultError)
{
  // scanning with qt script
  QJSEngine engine;
  QJSValue imageValue = engine.newQObject(convImage);
  QQmlEngine::setObjectOwnership(convImage, QQmlEngine::CppOwnership);

  engine.globalObject().setProperty("image", imageValue);
  QString scriptModified = script;

  scriptModified = scriptModified
                   .replace("image.addPoint", "addImagePoint")
                   .replace("image.width", "__internal_image_width")
                   .replace("image.height", "__internal_image_height");
  QString scriptTemplate = ConverterHelper::scanScriptTemplate();
  scriptModified = scriptTemplate.arg(scriptModified);

  int startPosition = 0;

  {
    QTextStream stream(&scriptTemplate, QIODevice::ReadOnly);
    QString scriptLine;
    int counter = 0;

    do {
      scriptLine = stream.readLine();

      if (scriptLine.contains("%1")) {
        startPosition = counter;
        break;
      }

      counter++;
    } while (!scriptLine.isNull());
  }

  QJSValue resultValue = engine.evaluate(scriptModified);

  if (convImage->scanTerminated()) {
    *resultError = QString("Script abort requested. Points count: %1").arg(convImage->pointsCount());
  } else if (resultValue.isError()) {
    int line = resultValue.property("lineNumber").toInt();
    *resultError = QString("Uncaught exception at line %1 : %2").arg(line - startPosition).arg(resultValue.toString());
  } else if (convImage->pointsCount() == 0) {
    *resultError = QString("Empty output");
  } else {
    *resultError = QString();
  }
}

void ConverterHelper::convertPixelsByScript(const QString &script, QVector<quint32> *data, QString *resultError)
{
  QString scriptTemplate = ConverterHelper::pixelsScriptTemplate();
  int startPosition = 0;

  {
    QTextStream stream(&scriptTemplate, QIODevice::ReadOnly);
    QString scriptLine;
    int counter = 0;

    do {
      scriptLine = stream.readLine();

      if (scriptLine.contains("%1")) {
        startPosition = counter;
        break;
      }

      counter++;
    } while (!scriptLine.isNull());
  }

  // scanning with qt script
  QJSEngine engine;
  ConvImagePixels pixelsData(data);
  QJSValue pixelsDataValue = engine.newQObject(&pixelsData);
  QQmlEngine::setObjectOwnership(&pixelsData, QQmlEngine::CppOwnership);

  engine.globalObject().setProperty("data", pixelsDataValue);
  QString scriptModified = scriptTemplate.arg(script);

  QJSValue resultValue = engine.evaluate(scriptModified);

  if (pixelsData.processTerminated()) {
    *resultError = QString("Script abort requested.");
  } else if (resultValue.isError()) {
    int line = resultValue.property("lineNumber").toInt();
    *resultError = QString("Uncaught exception at line %1 : %2").arg(line - startPosition).arg(resultValue.toString());
  } else {
    *resultError = QString();
  }

  if (resultError->isEmpty()) {
    *resultError = "";
  }

  if (resultError->isEmpty()) {
    pixelsData.getResults(data);
  }
}

void ConverterHelper::processPixels(Settings::Presets::Preset *preset, QVector<quint32> *data)
{
  if (preset != nullptr && data != nullptr) {
    for (int i = 0; i < data->size(); i++) {
      quint32 value = data->at(i);
      quint32 valueNew = 0;

      for (int j = 0; j < preset->matrix()->operationsCount(); j++) {
        quint32 mask;
        int shift;
        bool left;
        preset->matrix()->operation(j, &mask, &shift, &left);

        if (left) {
          valueNew |= (value & mask) << shift;
        } else {
          valueNew |= (value & mask) >> shift;
        }
      }

      if (preset->matrix()->operationsCount() == 0) {
        valueNew = value;
      }

      valueNew &= preset->matrix()->maskAnd();
      valueNew |= preset->matrix()->maskOr();
      data->replace(i, valueNew);
    }
  }
}

void ConverterHelper::packData(
  Settings::Presets::Preset *preset,
  QVector<quint32> *inputData, int inputWidth, int inputHeight,
  QVector<quint32> *outputData,
  int *outputWidth, int *outputHeight)
{
  *outputHeight = inputHeight;
  outputData->clear();

  int resultWidth = 0;
  int rowLength = 0;

  if (preset->image()->splitToRows()) {
    if (preset->prepare()->bandScanning()) {
      // non-standard row width

      /* Number_Of_Rows equals to Height * Number_Of_Columns
       * (bands used)
       *
       * ========  ========  ========  ========  ========
       * ========  ========  ========  ========  ========
       * ========  ========  ========  ========  ========
       * ========  ========  ========  ========  ========
       * ========  ========  ========  ========  ========
       */

      for (int row = 0; row < inputHeight; row++) {
        // start of row in inputData
        int start = row * inputWidth;
        // get row data packed
        ConverterHelper::packDataRow(preset, inputData, start, inputWidth, outputData, &rowLength);
        // get row blocks count
        resultWidth = qMax(resultWidth, rowLength);
      }

      *outputWidth = resultWidth;
    } else {
      /* Number_Of_Rows equals to Height
       *
       * ========================================
       * ========================================
       * ========================================
       * ========================================
       * ========================================
       */

      // process each standard row
      for (int y = 0; y < inputHeight; y++) {
        // start of row in inputData
        int start = y * inputWidth;
        // get row data packed
        ConverterHelper::packDataRow(preset, inputData, start, inputWidth, outputData, &rowLength);
        // get row blocks count
        resultWidth = qMax(resultWidth, rowLength);
      }

      *outputWidth = resultWidth;
    }
  } else {
    // All data in one row

    // process entire data
    ConverterHelper::packDataRow(preset, inputData, 0, inputData->size(), outputData, &rowLength);
    // get blocks count
    *outputWidth = rowLength;
    *outputHeight = 1;
  }
}

void ConverterHelper::reorder(
  Settings::Presets::Preset *preset,
  QVector<quint32> *inputData,
  int inputWidth,
  int inputHeight,
  QVector<quint32> *outputData,
  int *outputWidth,
  int *outputHeight)
{
  for (int i = 0; i < inputData->size(); i++) {
    quint32 value = inputData->at(i);
    quint32 valueNew = 0;

    for (int j = 0; j < preset->reordering()->operationsCount(); j++) {
      quint32 mask;
      int shift;
      bool left;
      preset->reordering()->operation(j, &mask, &shift, &left);

      if (left) {
        valueNew |= (value & mask) << shift;
      } else {
        valueNew |= (value & mask) >> shift;
      }
    }

    if (preset->reordering()->operationsCount() == 0) {
      valueNew = value;
    }

    outputData->append(valueNew);
  }

  *outputWidth = inputWidth;
  *outputHeight = inputHeight;
}

void ConverterHelper::compressData(Settings::Presets::Preset *preset,
                                   QVector<quint32> *inputData,
                                   int inputWidth, int inputHeight,
                                   QVector<quint32> *outputData,
                                   int *outputWidth, int *outputHeight)
{
  if (preset->image()->compressionRle()) {
    Utils::Compression::RleCompressor compressor;
    compressor.compress(inputData, preset->image()->blockSize(), outputData, preset->image()->compressionRleMinLength());
    *outputWidth = outputData->size();
    *outputHeight = 1;
  } else {
    for (int i = 0; i < inputData->size(); i++) {
      outputData->append(inputData->at(i));
    }

    *outputWidth = inputWidth;
    *outputHeight = inputHeight;
  }
}

void ConverterHelper::prepareImage(Settings::Presets::Preset *preset, const QImage *source, QImage *result)
{
  if (source != nullptr) {
    QImage im = *source;

    if (preset->prepare()->inverse()) {
      im.invertPixels();
    }

    *result = im;
  }
}

void ConverterHelper::createImagePreview(Settings::Presets::Preset *preset, QImage *source, QImage *result)
{
  if (source != nullptr) {
    QImage im = *source;

    if (preset->prepare()->inverse()) {
      im.invertPixels();
    }

    // convert to mono/gray/color
    if (preset->prepare()->convType() == Parsing::Conversion::Options::ConversionType::Monochrome) {
      switch (preset->prepare()->monoType()) {
        case Parsing::Conversion::Options::MonochromeType::Edge:
          ConverterHelper::makeMonochrome(im, preset->prepare()->edge());
          break;

        case Parsing::Conversion::Options::MonochromeType::DiffuseDither:
          im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
          im = im.convertToFormat(QImage::Format_ARGB32);
          break;

        case Parsing::Conversion::Options::MonochromeType::OrderedDither:
          im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
          im = im.convertToFormat(QImage::Format_ARGB32);
          break;

        case Parsing::Conversion::Options::MonochromeType::ThresholdDither:
          im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
          im = im.convertToFormat(QImage::Format_ARGB32);
          break;
      }
    } else if (preset->prepare()->convType() == Parsing::Conversion::Options::ConversionType::Grayscale) {
      ConverterHelper::makeGrayscale(im);
    }

    // mask used source data bits
    {
      // create mask
      quint32 mask = 0;

      switch (preset->prepare()->convType()) {
        case Parsing::Conversion::Options::ConversionType::Monochrome: {
          quint32 opMask;
          int opShift;
          bool opLeft;

          for (int i = 0; i < preset->matrix()->operationsCount(); i++) {
            preset->matrix()->operation(i, &opMask, &opShift, &opLeft);

            if (opMask != 0) {
              mask = 0xffffffff;
              break;
            }
          }

          if (preset->matrix()->operationsCount() == 0) {
            mask = 0xffffffff;
          }

          break;
        }

        case Parsing::Conversion::Options::ConversionType::Grayscale: {
          quint32 opMask;
          int opShift;
          bool opLeft;

          for (int i = 0; i < preset->matrix()->operationsCount(); i++) {
            preset->matrix()->operation(i, &opMask, &opShift, &opLeft);
            quint8 byte1 = (opMask >> 0) & 0xff;
            quint8 byte2 = (opMask >> 8) & 0xff;
            quint8 byte3 = (opMask >> 16) & 0xff;
            quint8 byte4 = (opMask >> 24) & 0xff;
            quint32 all = byte1 | byte2 | byte3 | byte4;
            mask |= all;
            mask |= all << 8;
            mask |= all << 16;
            mask |= all << 24;
          }

          if (preset->matrix()->operationsCount() == 0) {
            mask = 0xffffffff;
          }

          break;
        }

        case Parsing::Conversion::Options::ConversionType::Custom:
        case Parsing::Conversion::Options::ConversionType::Color: {
          quint32 opMask;
          int opShift;
          bool opLeft;

          for (int i = 0; i < preset->matrix()->operationsCount(); i++) {
            preset->matrix()->operation(i, &opMask, &opShift, &opLeft);
            mask |= opMask;
          }

          if (preset->matrix()->operationsCount() == 0) {
            mask = 0xffffffff;
          }

          // save alpha bits when alpha channel not used
          if ((mask & 0xff000000) == 0) {
            mask |= 0xff000000;
          }

          break;
        }
      }

      // apply mask
      for (int x = 0; x < im.width(); x++) {
        for (int y = 0; y < im.height(); y++) {
          QRgb value = im.pixel(x, y);
          value &= mask;
          im.setPixel(x, y, value);
        }
      }
    }

    *result = im;
  }
}

QString ConverterHelper::uint2string(Settings::Presets::DataNumeralSystem numeralSystem, Settings::Presets::DataBlockSize blockSize, quint32 value)
{
  int num;

  switch (numeralSystem) {
    case Settings::Presets::DataNumeralSystem::Decimal: {
      QString result;
      result.setNum(value);
      return result;
    }

    case Settings::Presets::DataNumeralSystem::Binary: {
      num = 0;
      break;
    }

    case Settings::Presets::DataNumeralSystem::Octal: {
      num = 1;
      break;
    }

    case Settings::Presets::DataNumeralSystem::Hexadecimal: {
      num = 2;
      break;
    }

    default: {
      return QString();
    }
  }

  QChar temp[40];
  static const QChar table[] = {
    QChar('0'), QChar('1'), QChar('2'), QChar('3'),
    QChar('4'), QChar('5'), QChar('6'), QChar('7'),
    QChar('8'), QChar('9'), QChar('a'), QChar('b'),
    QChar('c'), QChar('d'), QChar('e'), QChar('f')
  };
  static const QChar end = QChar('\0');

  static const int lengths[3][4] = {
    { 8, 16, 24, 32 },
    { 3, 6, 8, 11 },
    { 2, 4, 6, 8 }
  };

  static const quint32 masks[3] = {
    0x1ul,
    0x7ul,
    0xful
  };

  static const int shifts[3] = {
    1,
    3,
    4
  };

  static const quint32 limits[4] = {
    0xfful,
    0xfffful,
    0xfffffful,
    0xfffffffful
  };

  value &= limits[static_cast<int>(blockSize)];

  int length = lengths[num][static_cast<int>(blockSize)];
  quint32 mask = masks[num];
  int shift = shifts[num];
  int i;

  for (i = 0; i < length; i++) {
    temp[length - i - 1] = table[value & mask];
    value = value >> shift;
  }

  temp[i] = end;

  return QString(temp);
}

QString ConverterHelper::dataToString(
  Settings::Presets::Preset *preset,
  QVector<quint32> *data, int width, int height)
{
  Q_UNUSED(height)
  QString result, converted;

  int blockPerLine = preset->image()->blocksPerLine();

  Settings::Presets::DataBlockSize blockSize = preset->image()->blockSize();
  QString prefix = preset->image()->blockPrefix();
  QString suffix = preset->image()->blockSuffix();
  QString delimiter = preset->image()->blockDelimiter();
  Settings::Presets::DataNumeralSystem numeralSystem = preset->image()->numeralSystem();

  for (auto i = 0; i < data->size(); i++) {
    if (blockPerLine > 0) {
      if ((i > 0) && ((i % blockPerLine) == 0)) {
        result.append("\n");
      }
    }

    quint32 value = data->at(i);
    converted = uint2string(numeralSystem, blockSize, value);
    result += prefix + converted + suffix + delimiter;
  }

  result.truncate(result.length() - delimiter.length());

  return result;
}

QString ConverterHelper::previewDataToString(Settings::Presets::Preset *preset, const QVector<quint32> *data, int width, int height)
{
  QString result;

  QString prefix = preset->image()->previewPrefix();
  QString suffix = preset->image()->previewSuffix();
  QString delimiter = preset->image()->previewDelimiter();
  QStringList levels = preset->image()->previewLevels().split(QRegExp("[\r\n]"), Qt::SplitBehaviorFlags::SkipEmptyParts);

  int levelsCount = levels.length();

  if (levelsCount < 2) {
    return QString();
  }

  if (levelsCount > 256) {
    levelsCount = 256;
  }

  int levelStep = 256 / levelsCount;

  bool completed = false;

  for (int y = 0; y < height && !completed; y++) {
    if (y > 0) {
      result.append("\n");
    }

    result.append(prefix);

    for (int x = 0; x < width && !completed; x++) {
      // control index limits for compressed data
      int index = y * width + x;

      if (index >= data->size()) {
        completed = true;
        break;
      }

      QRgb value = data->at(index);
      int gray = qGray(value);
      int levelIndex = gray / levelStep;

      if (levelIndex >= levelsCount) {
        levelIndex = levelsCount - 1;
      }

      QString converted = levels.at(levelIndex);
      result.append(converted % delimiter);
    }

    result.append(suffix);
  }

  result.truncate(result.length() - delimiter.length());

  return result;
}

QString ConverterHelper::scanScript(Settings::Presets::Preset *preset)
{
  QString result;
  const Settings::Presets::PrepareOptions *prepare = preset->prepare();

  if (prepare->useCustomScanScript()) {
    result = prepare->customScanScript();
  } else {
    static const QString scripts[] = {
      ":/scan_scripts/t2b_b", // 0
      ":/scan_scripts/t2b_b_b",
      ":/scan_scripts/t2b_f",
      ":/scan_scripts/t2b_f_b",

      ":/scan_scripts/b2t_b", // 4
      ":/scan_scripts/b2t_b_b",
      ":/scan_scripts/b2t_f",
      ":/scan_scripts/b2t_f_b",

      ":/scan_scripts/l2r_b", // 8
      ":/scan_scripts/l2r_b_b",
      ":/scan_scripts/l2r_f",
      ":/scan_scripts/l2r_f_b",

      ":/scan_scripts/r2l_b", // 12
      ":/scan_scripts/r2l_b_b",
      ":/scan_scripts/r2l_f",
      ":/scan_scripts/r2l_f_b"
    };

    int index = 0;

    switch (prepare->scanMain()) {
      case Parsing::Conversion::Options::ScanMainDirection::TopToBottom: {
        index = 0;
        break;
      }

      case Parsing::Conversion::Options::ScanMainDirection::BottomToTop: {
        index = 4;
        break;
      }

      case Parsing::Conversion::Options::ScanMainDirection::LeftToRight: {
        index = 8;
        break;
      }

      case Parsing::Conversion::Options::ScanMainDirection::RightToLeft: {
        index = 12;
        break;
      }
    }

    if (prepare->scanSub() == Parsing::Conversion::Options::ScanSubDirection::Forward) {
      index += 2;
    }

    if (prepare->bandScanning()) {
      index += 1;
    }

    if (index >= 0 && index < 16) {
      QFile file_script(scripts[index]);

      if (file_script.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file_script);
        result = stream.readAll();
        file_script.close();
      }
    }
  }

  return result;
}

QString ConverterHelper::scanScriptTemplate()
{
  QFile file_script(":/scan_scripts/scan_template");
  QString result = QString();

  if (file_script.open(QIODevice::ReadOnly)) {
    QTextStream stream(&file_script);
    result = stream.readAll();
    file_script.close();
  }

  return result;
}

QString ConverterHelper::pixelsScript(Settings::Presets::Preset *preset)
{
  QString result = preset->prepare()->customPreprocessScript();

  if (result.isEmpty()) {
    static const QString scriptPath = ":/scan_scripts/pixels_example";
    QFile file_script(scriptPath);

    if (file_script.open(QIODevice::ReadOnly)) {
      QTextStream stream(&file_script);
      result = stream.readAll();
      file_script.close();
    }
  }

  return result;
}

QString ConverterHelper::pixelsScriptTemplate()
{
  QFile file_script(":/scan_scripts/pixels_template");
  QString result = QString();

  if (file_script.open(QIODevice::ReadOnly)) {
    QTextStream stream(&file_script);
    result = stream.readAll();
    file_script.close();
  }

  return result;
}

void ConverterHelper::makeMonochrome(QImage &image, int edge)
{
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing, false);
  edge &= 0x000000ff;

  for (int x = 0; x < image.width(); x++) {
    for (int y = 0; y < image.height(); y++) {
      QRgb value = image.pixel(x, y);
      int alpha = qAlpha(value);

      if (qGray(value) < edge) {
        painter.setPen(QColor(0, 0, 0, alpha));
      } else {
        painter.setPen(QColor(255, 255, 255, alpha));
      }

      painter.drawPoint(x, y);
    }
  }
}

void ConverterHelper::makeGrayscale(QImage &image)
{
  for (int x = 0; x < image.width(); x++) {
    for (int y = 0; y < image.height(); y++) {
      QRgb value = image.pixel(x, y);
      int gray = qGray(value);
      int alpha = qAlpha(value);
      QColor color = QColor(gray, gray, gray, alpha);
      value = color.rgba();
      image.setPixel(x, y, value);
    }
  }
}

void ConverterHelper::packDataRow(Settings::Presets::Preset *preset, QVector<quint32> *inputData, int start, int count, QVector<quint32> *outputData, int *rowLength)
{
  *rowLength = 0;

  if (preset != nullptr && inputData != nullptr && outputData != nullptr) {
    BitStream stream(preset, inputData, start, count);

    while (!stream.eof()) {
      quint32 value = stream.next();

      if (preset->image()->bytesOrder() == Parsing::Conversion::Options::BytesOrder::BigEndian) {
        value = ConverterHelper::toBigEndian(preset, value);
      }

      outputData->append(value);
      (*rowLength)++;
    }
  }
}

quint32 ConverterHelper::toBigEndian(Settings::Presets::Preset *preset, quint32 value)
{
  quint8 src1, src2, src3, src4;
  src1 = value & 0xff;
  src2 = (value >> 8) & 0xff;
  src3 = (value >> 16) & 0xff;
  src4 = (value >> 24) & 0xff;

  quint32 result = 0;

  switch (preset->image()->blockSize()) {
    case Parsing::Conversion::Options::DataBlockSize::Data32:
      result |= src1 << 24;
      result |= src2 << 16;
      result |= src3 << 8;
      result |= src4;
      break;

    case Parsing::Conversion::Options::DataBlockSize::Data24:
      result |= src1 << 16;
      result |= src2 << 8;
      result |= src3;
      break;

    case Parsing::Conversion::Options::DataBlockSize::Data16:
      result |= src1 << 8;
      result |= src2;
      break;

    case Parsing::Conversion::Options::DataBlockSize::Data8:
      result = src1;
      break;
  }

  return result;
}

void ConverterHelper::getImageDataSize(Settings::Presets::PrepareOptions *prepare, ConvImageScan *convImage, int *width, int *height)
{
  enum class LineOrientation {
    Unknown,
    Horizontal,
    Vertical
  };

  LineOrientation orientation = LineOrientation::Unknown;
  QPoint point1 = convImage->pointAt(0);
  QPoint point2 = convImage->pointAt(1);

  if (!prepare->useCustomScanScript()) {
    switch (prepare->scanMain()) {
      case Parsing::Conversion::Options::ScanMainDirection::LeftToRight:
      case Parsing::Conversion::Options::ScanMainDirection::RightToLeft: {
        orientation = LineOrientation::Vertical;
        break;
      }

      case Parsing::Conversion::Options::ScanMainDirection::TopToBottom:
      case Parsing::Conversion::Options::ScanMainDirection::BottomToTop: {
        orientation = LineOrientation::Horizontal;
        break;
      }
    }
  }

  if (orientation == LineOrientation::Unknown) {
    if ((qAbs(point1.x() - point2.x()) == 1) && (point1.y() == point2.y())) {
      orientation = LineOrientation::Horizontal;
    } else if ((qAbs(point1.y() - point2.y()) == 1) && (point1.x() == point2.x())) {
      orientation = LineOrientation::Vertical;
    }
  }

  switch (orientation) {
    case LineOrientation::Horizontal: {
      // get length of first horizontal line
      QPoint point = point1;

      for (int i = 0; i < convImage->pointsCount(); i++) {
        if (convImage->pointAt(i).y() != point.y()) {
          break;
        }

        point = convImage->pointAt(i);
      }

      *width = qAbs(point.x() - point1.x()) + 1;
      *height = convImage->pointsCount() / (*width);
      break;
    }

    case LineOrientation::Vertical: {
      // get length of first vertical line
      QPoint point = point1;

      for (int i = 0; i < convImage->pointsCount(); i++) {
        if (convImage->pointAt(i).x() != point.x()) {
          break;
        }

        point = convImage->pointAt(i);
      }

      *width = qAbs(point.y() - point1.y()) + 1;
      *height = convImage->pointsCount() / (*width);
      break;
    }

    case LineOrientation::Unknown: {
      *width = convImage->pointsCount();
      *height = 1;
      break;
    }
  }
}

} // namespace Conversion
} // namespace Parsing
