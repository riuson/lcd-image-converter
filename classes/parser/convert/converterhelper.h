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

#ifndef CONVERTERHELPER_H
#define CONVERTERHELPER_H

#include "preset.h"

class QImage;

namespace Settings
{
namespace Presets
{
class Preset;
}
}

namespace Parsing
{
namespace Conversion
{
class ConvImageScan;

class ConverterHelper
{
public:
  // gets RGB array of pixels
  // also convert images to monochrome or grayscale (r = g = b = qGray(pixel))
  static void pixelsData(
    Settings::Presets::PrepareOptions *prepare,
    const QString &scanScript,
    const QString &pixelScript,
    const QImage *image,
    QVector<quint32> *data,
    int *width,
    int *height);
  static void collectPoints(ConvImageScan *convImage, const QString &script, QString *resultError);
  // apply custom bits processing
  static void convertPixelsByScript(const QString &script, QVector<quint32> *data, QString *resultError);
  // apply masks and bits reorder
  static void processPixels(Settings::Presets:: Preset *preset, QVector<quint32> *data);
  // pack data
  static void packData(
    Settings::Presets::Preset *preset,
    QVector<quint32> *inputData,
    int inputWidth,
    int inputHeight,
    QVector<quint32> *outputData,
    int *outputWidth,
    int *outputHeight);
  // bits reordering
  static void reorder(
    Settings::Presets::Preset *preset,
    QVector<quint32> *inputData, int inputWidth, int inputHeight,
    QVector<quint32> *outputData, int *outputWidth, int *outputHeight);

  static void compressData(
          Settings::Presets::Preset *preset,
          QVector<quint32> *inputData,
          QVector<quint32> *outputData);

  static void prepareImage(Settings::Presets::Preset *preset, const QImage *source, QImage *result);
  static void createImagePreview(Settings::Presets::Preset *preset, QImage *source, QImage *result);

  static QString uint2string(Settings::Presets::DataNumeralSystem numeralSystem, Settings::Presets::DataBlockSize blockSize, quint32 value);
  static QString dataToString(Settings::Presets::Preset *preset,
                              QVector<quint32> *data);
  static QString previewDataToString(Settings::Presets::Preset *preset,
                                     const QVector<quint32> *data, int width, int height);

  static QString scanScript(Settings::Presets::Preset *preset);
  static QString scanScriptTemplate();
  static QString pixelsScript(Settings::Presets::Preset *preset);
  static QString pixelsScriptTemplate();

  static void makeGrayscale(QImage &image);
private:
  static void makeMonochrome(QImage &image, int edge);
  // make r = g = b = qGray(pixel)
  static void packDataRow(
    Settings::Presets::Preset *preset,
    QVector<quint32> *inputData,
    int start,
    int count,
    QVector<quint32> *outputData,
    int *rowLength);
  static quint32 toBigEndian(Settings::Presets::Preset *preset, quint32 value);
  static void getImageDataSize(Settings::Presets::PrepareOptions *prepare, ConvImageScan *convImage, int *width, int *height);
};

} // namespace Conversion
} // namespace Parsing

#endif // CONVERTERHELPER_H
