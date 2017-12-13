/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2014 riuson
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

#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QTextCodec>
#include <QFontDatabase>
#include <QFontMetrics>
#include "modeconvertfont.h"
#include "fontdocument.h"
#include "datacontainer.h"
#include "preset.h"
#include "templateoptions.h"
#include "fontparameters.h"
#include "bitmaphelper.h"

namespace CommandLine
{

ModeConvertFont::ModeConvertFont(QCommandLineParser &parser, QObject *parent) :
  ModeParserBase(parser, parent)
{
  this->mFontFamily = "Times";
  this->mFontSize = 14;
  this->mFontMonospaced = false;
  this->mFontStyle = "Normal";
  this->mFontAntiAliasing = false;
  this->mFontCharactersList = "0123456789ABCDEFabcdef";

  this->mFontCharactersEncoding = "UTF-8";
  this->mFontCharactersRange = QString();
  this->mFontCharactersBigEndian = false;

  this->mInputFilename = QString();
  this->mOutputFilename = QString();
  this->mTemplateFilename = QString();
  this->mDocumentName = QString();
  this->mPresetName = QString();

  this->mSubMode = SubMode::None;
}

QString ModeConvertFont::modeName()
{
  return "convert-font";
}

void ModeConvertFont::fillParser() const
{
  // --family="Ubuntu"
  QCommandLineOption familyOption(QStringList() << "family",
                                  QCoreApplication::translate("CmdLineParser", "<Family> of new font."),
                                  QCoreApplication::translate("CmdLineParser", "family"));
  this->mParser.addOption(familyOption);

  // --size=14
  QCommandLineOption sizeOption(QStringList() << "size",
                                QCoreApplication::translate("CmdLineParser", "Font <size>."),
                                QCoreApplication::translate("CmdLineParser", "size"));
  this->mParser.addOption(sizeOption);

  // --mono
  QCommandLineOption monoOption(QStringList() << "mono" << "monospaced",
                                QCoreApplication::translate("CmdLineParser", "Make monospaced font."));
  this->mParser.addOption(monoOption);

  // --style
  QCommandLineOption styleOption(QStringList() << "style",
                                 QCoreApplication::translate("CmdLineParser", "Font <style>, Regular, Bold, Italic or Bold Italic."));
  this->mParser.addOption(styleOption);

  // --antialiasing
  QCommandLineOption antialiasingOption(QStringList() << "antialiasing",
                                        QCoreApplication::translate("CmdLineParser", "Use antialiasing."));
  this->mParser.addOption(antialiasingOption);

  // --foreground
  QCommandLineOption foregroundOption(QStringList() << "foreground",
                                      QCoreApplication::translate("CmdLineParser", "Foreground color in hex format."),
                                      QCoreApplication::translate("CmdLineParser", "color"));
  this->mParser.addOption(foregroundOption);

  // --background
  QCommandLineOption backgroundOption(QStringList() << "background",
                                      QCoreApplication::translate("CmdLineParser", "Background color in hex format."),
                                      QCoreApplication::translate("CmdLineParser", "color"));
  this->mParser.addOption(backgroundOption);

  // --chars-list
  QCommandLineOption charsListOption(QStringList() << "chars-list",
                                     QCoreApplication::translate("CmdLineParser", "Characters, what included to the font."),
                                     QCoreApplication::translate("CmdLineParser", "characters"));
  this->mParser.addOption(charsListOption);

  // --chars-range
  QCommandLineOption charsRangeOption(QStringList() << "chars-range",
                                      QCoreApplication::translate("CmdLineParser", "Characters range, for example \"32-255\"."),
                                      QCoreApplication::translate("CmdLineParser", "range"));
  this->mParser.addOption(charsRangeOption);

  // --chars-encoding
  QCommandLineOption charsEncodingOption(QStringList() << "chars-encoding",
                                         QCoreApplication::translate("CmdLineParser", "Characters encoding, for example \"UTF-8\"."),
                                         QCoreApplication::translate("CmdLineParser", "encoding"));
  this->mParser.addOption(charsEncodingOption);

  // --big-endian
  QCommandLineOption endianessOption(QStringList() << "big-endian",
                                     QCoreApplication::translate("CmdLineParser", "Use big-endian instead of little-endian."));
  this->mParser.addOption(endianessOption);

  // --input=/temp/1.xml
  QCommandLineOption inputOption(QStringList() << "i" << "input",
                                 QCoreApplication::translate("CmdLineParser", "Full <path> to font document in xml format."),
                                 QCoreApplication::translate("CmdLineParser", "path"));
  this->mParser.addOption(inputOption);

  // --output=/temp/1.c
  QCommandLineOption outputOption(QStringList() << "o" << "output",
                                  QCoreApplication::translate("CmdLineParser", "Full <path> to output result."),
                                  QCoreApplication::translate("CmdLineParser", "path"));
  this->mParser.addOption(outputOption);

  // --template=/temp/image.tmpl
  QCommandLineOption templateOption(QStringList() << "template",
                                    QCoreApplication::translate("CmdLineParser", "Full <path> to template file, used in conversion. [Optional]"),
                                    QCoreApplication::translate("CmdLineParser", "path"));
  this->mParser.addOption(templateOption);

  // --doc-name=testImage
  QCommandLineOption documentNameOption(QStringList() << "doc-name",
                                        QCoreApplication::translate("CmdLineParser", "Document name."),
                                        QCoreApplication::translate("CmdLineParser", "name"));
  this->mParser.addOption(documentNameOption);

  // --preset-name=lcdR4G5B4
  QCommandLineOption presetOption(QStringList() << "preset-name",
                                  QCoreApplication::translate("CmdLineParser", "Output preset <name> from predefined presets in application settings."),
                                  QCoreApplication::translate("CmdLineParser", "name"));
  this->mParser.addOption(presetOption);
}

bool ModeConvertFont::collectArguments()
{
  this->mFontFamily = this->mParser.value("family");
  bool sizeOk;
  this->mFontSize = this->mParser.value("size").toInt(&sizeOk);
  this->mFontMonospaced = this->mParser.isSet("monospaced");
  this->mFontStyle = this->mParser.value("style");
  this->mFontAntiAliasing = this->mParser.isSet("antialiasing");
  this->mForeground = this->mParser.value("foreground");
  this->mBackground = this->mParser.value("background");

  this->mFontCharactersList = this->mParser.value("chars-list");
  this->mFontCharactersRange = this->mParser.value("chars-range");

  this->mFontCharactersEncoding = this->mParser.value("chars-encoding");
  this->mFontCharactersBigEndian = this->mParser.isSet("big-endian");

  this->mInputFilename = this->mParser.value("input");
  this->mOutputFilename = this->mParser.value("output");
  this->mTemplateFilename = this->mParser.value("template");
  this->mDocumentName = this->mParser.value("doc-name");
  this->mPresetName = this->mParser.value("preset-name");

  if (this->mPresetName.isEmpty() || this->mOutputFilename.isEmpty()) {
    this->mSubMode = SubMode::None;
    return false;
  }

  if (!this->mInputFilename.isEmpty() &&
      QFile::exists(this->mInputFilename)) {
    this->mSubMode = SubMode::FromXmlDocument;
    return true;
  }

  // check dodocument name
  this->mDocumentNameWS = this->mDocumentName.remove(QRegExp(QRegExp("\\W", Qt::CaseInsensitive)));

  if (this->mDocumentName.isEmpty() || this->mDocumentNameWS.isEmpty()) {
    return false;
  }

  if (!this->mFontFamily.isEmpty() &&
      sizeOk &&
      !this->mFontCharactersList.isEmpty()) {
    this->mSubMode = SubMode::FromCharactersList;
    return true;
  }

  if (!this->mFontFamily.isEmpty() &&
      sizeOk &&
      (!this->mFontCharactersRange.isEmpty() && !this->mFontCharactersEncoding.isEmpty())) {
    this->mSubMode = SubMode::FromCharactersRange;
    return true;
  }

  return false;
}

int ModeConvertFont::process()
{
  // check preset exists
  if (Preset::presetsList().contains(this->mPresetName)) {
    Data::Containers::FontDocument fontDocument;

    if (this->mSubMode == SubMode::FromXmlDocument) {
      if (!fontDocument.load(this->mInputFilename)) {
        return 1;
      }
    }

    if ((this->mSubMode == SubMode::FromCharactersList) || (this->mSubMode == SubMode::FromCharactersRange)) {
      if (this->mSubMode == SubMode::FromCharactersRange) {
        this->mFontCharactersList = this->createCharsList(
                                      this->mFontCharactersRange,
                                      this->mFontCharactersEncoding,
                                      this->mFontCharactersBigEndian);
      }

      Data::Containers::FontParameters parameters;
      parameters.family = this->mFontFamily;
      parameters.style = this->mFontStyle;
      parameters.size = this->mFontSize;
      parameters.monospaced = this->mFontMonospaced;
      parameters.antiAliasing = this->mFontAntiAliasing;

      // get ascent/descent
      {
        QFontDatabase fonts;
        QFont font = fonts.font(parameters.family, parameters.style, parameters.size);
        QFontMetrics metrics(font);
        parameters.ascent = metrics.ascent();
        parameters.descent = metrics.descent();
      }

      // colors
      {
        bool colorOk;
        quint32 rgbValue;

        // foreground
        rgbValue = this->mForeground.toUInt(&colorOk, 16);

        if (colorOk) {
          parameters.foreground = BitmapHelper::fromRgba(QRgb(rgbValue));
        } else {
          parameters.foreground = QColor("black");
        }

        // background
        rgbValue = this->mBackground.toUInt(&colorOk, 16);

        if (colorOk) {
          parameters.background = BitmapHelper::fromRgba(QRgb(rgbValue));
        } else {
          parameters.background = QColor("white");
        }
      }

      fontDocument.setFontCharacters(
        this->mFontCharactersList,
        parameters);

      fontDocument.setDocumentName(this->mDocumentName);
    }

    fontDocument.dataContainer()->setCommonInfo("converted filename", QVariant(this->mOutputFilename));

    // save to output file
    QFile file(this->mOutputFilename);

    if (file.open(QFile::WriteOnly)) {
      Preset::setSelectedName(this->mPresetName);
      Preset preset;
      preset.load(this->mPresetName);

      // optional template file
      if (!this->mTemplateFilename.isEmpty() && QFile::exists(this->mTemplateFilename)) {
        preset.templates()->setFont(this->mTemplateFilename);
      }

      QString result = fontDocument.convert(&preset);

      file.write(result.toUtf8());
      file.close();

      if (fontDocument.outputFilename() != this->mOutputFilename) {
        fontDocument.setOutputFilename(this->mOutputFilename);
      }
    }

    return 0;
  }

  return 1;
}

QString ModeConvertFont::createCharsList(const QString &rangeStr,
    const QString &encoding,
    bool bigEndian) const
{
  QString result;

  QStringList rangeListStr = rangeStr.split(QRegExp("[\\.\\-\\ ]"), QString::SkipEmptyParts);

  if (rangeListStr.size() == 2) {
    bool ok;
    int from = rangeListStr.at(0).toInt(&ok);

    if (ok) {
      int to = rangeListStr.at(1).toInt(&ok);

      if (ok) {

        // from dialogfontrange.cpp
        if (from > to) {
          qSwap(from, to);
        }

        for (int i = from; i <= to; ++i) {
          int code = i;

          if (code > 0) {
            QByteArray array;

            while (code != 0) {
              if (bigEndian) {
                array.insert(0, (char)(code & 0xff));
              } else {
                array.append((char)(code & 0xff));
              }

              code = code >> 8;
            }

            QTextCodec *codec = QTextCodec::codecForName(encoding.toLatin1());
            QString str = codec->toUnicode(array);
            result += str;
          } else {
            result += QChar(QChar::Null);
          }
        }

        // end from

      }
    }
  }

  return result;
}

}
