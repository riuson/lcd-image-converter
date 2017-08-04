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

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 5, 0)
#define USE_JS_QJSENGINE
#else
#define USE_JS_QTSCRIPT
#endif // QT_VERSION

#include <QStringList>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QRegExp>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QTextStream>

#if defined(USE_JS_QTSCRIPT)
#include <QtScript/QScriptEngine>
#elif defined(USE_JS_QJSENGINE)
#include <QJSEngine>
#include <QQmlEngine>
#endif

#include "bitstream.h"
#include "bitmaphelper.h"
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "reorderingoptions.h"
#include "imageoptions.h"
#include "rlecompressor.h"
#include "convimage.h"

void ConverterHelper::pixelsData(PrepareOptions *prepare, const QString &script, const QImage *image, QVector<quint32> *data, int *width, int *height)
{
  if (image != NULL && data != NULL && width != NULL && height != NULL) {
    data->clear();

    QImage im = *image;

    *width = im.width();
    *height = im.height();

    // monochrome image needs special preprocessing
    ConversionType type = prepare->convType();

    if (type == ConversionTypeMonochrome) {
      MonochromeType monotype = prepare->monoType();
      int edge = prepare->edge();

      switch (monotype) {
        case MonochromeTypeEdge:
          ConverterHelper::makeMonochrome(im, edge);
          break;

        case MonochromeTypeDiffuseDither:
          im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
          break;

        case MonochromeTypeOrderedDither:
          im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
          break;

        case MonochromeTypeThresholdDither:
          im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
          break;
      }
    } else if (type == ConversionTypeGrayscale) {
      ConverterHelper::makeGrayscale(im);
    }

    {
      ConvImage *convImage = new ConvImage(&im);
      convImage->setBandSize(prepare->bandWidth());
      convImage->setUseBands(prepare->bandScanning());

      QString errorMessage;
      ConverterHelper::collectPoints(convImage, script, &errorMessage);

      if (convImage->pointsCount() > 2) {
        // find image data size
        QPoint point1 = convImage->pointAt(0);
        QPoint point2 = convImage->pointAt(1);

        // if horizontal lines
        if ((qAbs(point1.x() - point2.x()) == 1) && (point1.y() == point2.y())) {
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
        }
        // if vertical lines
        else if ((qAbs(point1.y() - point2.y()) == 1) && (point1.x() == point2.x())) {
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
        }
        // unknown
        else {
          *width = convImage->pointsCount();
          *height = 1;
        }

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
  }
}

void ConverterHelper::collectPoints(ConvImage *convImage, const QString &script, QString *resultError)
{
#if defined(USE_JS_QTSCRIPT)
  // scanning with qt script
  QScriptEngine engine;
  QScriptValue imageValue = engine.newQObject(convImage,
                            QScriptEngine::QtOwnership,
                            QScriptEngine::ExcludeSuperClassProperties | QScriptEngine::ExcludeSuperClassMethods);
  engine.globalObject().setProperty("image", imageValue);
  QScriptValue resultValue = engine.evaluate(script);

  if (engine.hasUncaughtException()) {
    int line = engine.uncaughtExceptionLineNumber();
    *resultError = QString("Uncaught exception at line %1 : %2").arg(line).arg(resultValue.toString());
  } else if (convImage->pointsCount() == 0) {
    *resultError = QString("Empty output");
  } else {
    *resultError = QString();
  }

#elif defined(USE_JS_QJSENGINE)
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

#endif
}

void ConverterHelper::processPixels(Preset *preset, QVector<quint32> *data)
{
  if (preset != NULL && data != NULL) {
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
  Preset *preset,
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
  Preset *preset,
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

void ConverterHelper::compressData(Preset *preset,
                                   QVector<quint32> *inputData,
                                   int inputWidth, int inputHeight,
                                   QVector<quint32> *outputData,
                                   int *outputWidth, int *outputHeight)
{
  if (preset->image()->compressionRle()) {
    RleCompressor compressor;
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

void ConverterHelper::prepareImage(Preset *preset, const QImage *source, QImage *result)
{
  if (source != NULL) {
    QImage im = *source;

    if (preset->prepare()->inverse()) {
      im.invertPixels();
    }

    *result = im;
  }
}

void ConverterHelper::createImagePreview(Preset *preset, QImage *source, QImage *result)
{
  if (source != NULL) {
    QImage im = *source;

    if (preset->prepare()->inverse()) {
      im.invertPixels();
    }

    // convert to mono/gray/color
    if (preset->prepare()->convType() == ConversionTypeMonochrome) {
      switch (preset->prepare()->monoType()) {
        case MonochromeTypeEdge:
          ConverterHelper::makeMonochrome(im, preset->prepare()->edge());
          break;

        case MonochromeTypeDiffuseDither:
          im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
          break;

        case MonochromeTypeOrderedDither:
          im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
          break;

        case MonochromeTypeThresholdDither:
          im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
          break;
      }
    } else if (preset->prepare()->convType() == ConversionTypeGrayscale) {
      ConverterHelper::makeGrayscale(im);
    }

    // mask used source data bits
    {
      // create mask
      quint32 mask = 0;

      switch (preset->prepare()->convType()) {
        case ConversionTypeMonochrome: {
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

        case ConversionTypeGrayscale: {
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

        case ConversionTypeColor: {
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

static inline QString uint2hex(DataBlockSize blockSize, quint32 value)
{
  QChar temp[10];
  static const QChar table[] = {
    QChar('0'), QChar('1'), QChar('2'), QChar('3'),
    QChar('4'), QChar('5'), QChar('6'), QChar('7'),
    QChar('8'), QChar('9'), QChar('a'), QChar('b'),
    QChar('c'), QChar('d'), QChar('e'), QChar('f')
  };
  static const QChar end = QChar('\0');

  switch (blockSize) {
    case Data8:
      temp[0] = table[(value >> 4) & 0x0000000f];
      temp[1] = table[(value >> 0) & 0x0000000f];
      temp[2] = end;
      break;

    case Data16:
      temp[0] = table[(value >> 12) & 0x0000000f];
      temp[1] = table[(value >> 8) & 0x0000000f];
      temp[2] = table[(value >> 4) & 0x0000000f];
      temp[3] = table[(value >> 0) & 0x0000000f];
      temp[4] = end;
      break;

    case Data24:
      temp[0] = table[(value >> 20) & 0x0000000f];
      temp[1] = table[(value >> 16) & 0x0000000f];
      temp[2] = table[(value >> 12) & 0x0000000f];
      temp[3] = table[(value >> 8) & 0x0000000f];
      temp[4] = table[(value >> 4) & 0x0000000f];
      temp[5] = table[(value >> 0) & 0x0000000f];
      temp[6] = end;
      break;

    case Data32:
      temp[0] = table[(value >> 28) & 0x0000000f];
      temp[1] = table[(value >> 24) & 0x0000000f];
      temp[2] = table[(value >> 20) & 0x0000000f];
      temp[3] = table[(value >> 16) & 0x0000000f];
      temp[4] = table[(value >> 12) & 0x0000000f];
      temp[5] = table[(value >> 8) & 0x0000000f];
      temp[6] = table[(value >> 4) & 0x0000000f];
      temp[7] = table[(value >> 0) & 0x0000000f];
      temp[8] = end;
      break;

    default:
      temp[0] = end;
      break;
  }

  return QString(temp);
}

QString ConverterHelper::dataToString(
  Preset *preset,
  QVector<quint32> *data, int width, int height)
{
  QString result, converted;

  DataBlockSize blockSize = preset->image()->blockSize();
  QString prefix = preset->image()->blockPrefix();
  QString suffix = preset->image()->blockSuffix();
  QString delimiter = preset->image()->blockDelimiter();

  if (preset->image()->splitToRows()) {
    bool completed = false;

    for (int y = 0; y < height && !completed; y++) {
      if (y > 0) {
        result.append("\n");
      }

      for (int x = 0; x < width && !completed; x++) {
        // control index limits for compressed data
        int index = y * width + x;

        if (index >= data->size()) {
          completed = true;
          break;
        }

        quint32 value = data->at(index);
        converted = uint2hex(blockSize, value);
        result += prefix + converted + suffix + delimiter;
      }
    }

    result.truncate(result.length() - delimiter.length());
  } else {
    bool completed = false;

    for (int i = 0; i < width && !completed; i++) {
      // control index limits for compressed data
      if (i >= data->size()) {
        completed = true;
        break;
      }

      quint32 value = data->at(i);
      converted = uint2hex(blockSize, value);
      result += prefix + converted + suffix + delimiter;
    }

    result.truncate(result.length() - delimiter.length());
  }

  return result;
}

QString ConverterHelper::scanScript(Preset *preset)
{
  QString result;
  const PrepareOptions *prepare = preset->prepare();

  if (prepare->useCustomScript()) {
    result = prepare->customScript();
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
      case TopToBottom: {
        index = 0;
        break;
      }

      case BottomToTop: {
        index = 4;
        break;
      }

      case LeftToRight: {
        index = 8;
        break;
      }

      case RightToLeft: {
        index = 12;
        break;
      }
    }

    if (prepare->scanSub() == Forward) {
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
  QFile file_script(":/scan_scripts/template");
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

void ConverterHelper::packDataRow(Preset *preset, QVector<quint32> *inputData, int start, int count, QVector<quint32> *outputData, int *rowLength)
{
  *rowLength = 0;

  if (preset != NULL && inputData != NULL && outputData != NULL) {
    BitStream stream(preset, inputData, start, count);

    while (!stream.eof()) {
      quint32 value = stream.next();

      if (preset->image()->bytesOrder() == BytesOrderBigEndian) {
        value = ConverterHelper::toBigEndian(preset, value);
      }

      outputData->append(value);
      (*rowLength)++;
    }
  }
}

quint32 ConverterHelper::toBigEndian(Preset *preset, quint32 value)
{
  quint8 src1, src2, src3, src4;
  src1 = value & 0xff;
  src2 = (value >> 8) & 0xff;
  src3 = (value >> 16) & 0xff;
  src4 = (value >> 24) & 0xff;

  quint32 result = 0;

  switch (preset->image()->blockSize()) {
    case Data32:
      result |= src1 << 24;
      result |= src2 << 16;
      result |= src3 << 8;
      result |= src4;
      break;

    case Data24:
      result |= src1 << 16;
      result |= src2 << 8;
      result |= src3;
      break;

    case Data16:
      result |= src1 << 8;
      result |= src2;
      break;

    case Data8:
      result = src1;
      break;
  }

  return result;
}

