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

#include "modeconvertimage.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include "preset.h"
#include "templateoptions.h"
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QImage>

namespace CommandLine
{

ModeConvertImage::ModeConvertImage(QCommandLineParser &parser, QObject *parent) :
  ModeParserBase(parser, parent)
{
}

QString ModeConvertImage::modeName()
{
  return "convert-image";
}

void ModeConvertImage::fillParser() const
{
  // --input=/temp/1.png
  QCommandLineOption inputOption(QStringList() << "i" << "input",
                                 QCoreApplication::translate("CmdLineParser", "Full <path> to source image in binary format."),
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

bool ModeConvertImage::collectArguments()
{
  this->mInputFilename = this->mParser.value("input");
  this->mOutputFilename = this->mParser.value("output");
  this->mTemplateFilename = this->mParser.value("template");
  this->mDocumentName = this->mParser.value("doc-name");
  this->mPresetName = this->mParser.value("preset-name");

  return (!this->mInputFilename.isEmpty() &&
          !this->mOutputFilename.isEmpty() &&
          !this->mDocumentName.isEmpty() &&
          !this->mPresetName.isEmpty());
}

int ModeConvertImage::process()
{
  // check input and template files exists
  if (QFile::exists(this->mInputFilename)) {
    // check dodocument name
    QString docNameWS = this->mDocumentName;
    docNameWS.remove(QRegExp("\\s"));

    if (!docNameWS.isEmpty()) {
      // check preset exists
      if (Settings::Presets::Preset::presetsList().contains(this->mPresetName)) {
        Settings::Presets::Preset::setSelectedName(this->mPresetName);

        // load image from input file
        QImage imageLoaded;

        if (imageLoaded.load(this->mInputFilename)) {
          QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

          Data::Containers::ImageDocument imageDocument;
          QStringList keys = imageDocument.dataContainer()->keys();

          // process all keys (1 in image document)
          QStringListIterator iterator(keys);

          while (iterator.hasNext()) {
            QString key = iterator.next();
            imageDocument.dataContainer()->setImage(key, &imageConverted);

            imageDocument.setDocumentName(docNameWS);
            imageDocument.dataContainer()->setCommonInfo("converted filename", QVariant(this->mOutputFilename));

            // save to output file
            QFile file(this->mOutputFilename);

            if (file.open(QFile::WriteOnly)) {
              Settings::Presets::Preset preset;
              preset.load(this->mPresetName);

              // optional template file
              if (!this->mTemplateFilename.isEmpty() && QFile::exists(this->mTemplateFilename)) {
                preset.templates()->setImage(this->mTemplateFilename);
              }

              QString result = imageDocument.convert(&preset);

              file.write(result.toUtf8());
              file.close();

              if (imageDocument.outputFilename() != this->mOutputFilename) {
                imageDocument.setOutputFilename(this->mOutputFilename);
              }
            }
          }

          return 0;
        }
      }
    }
  }

  return 1;
}

}
