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

#include "cmdline.h"
#include "preset.h"
#include "parser.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include "modeconvertimage.h"
#include "modeconvertfont.h"
#include "modehex2bin.h"
#include "appsettings.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QCommandLineParser>

namespace CommandLine
{

CmdLine::CmdLine(const QStringList &arguments, QObject *parent) :
  QObject(parent),
  mArguments(arguments)
{
  // information block
  this->mParser.setApplicationDescription("Tool to create image and font source files for embedded applications.");
  this->mParser.addHelpOption();
  this->mParser.addVersionOption();

  // required option:
  // --mode=convert-image
  QCommandLineOption optionMode(QStringList() << "m" << "mode",
                                QCoreApplication::translate("CmdLineParser", "Conversion mode for application, \"convert-image\", \"convert-font\" or \"hex2bin\"."),
                                QCoreApplication::translate("CmdLineParser", "mode"));
  this->mParser.addOption(optionMode);

  this->addApplicationOptions();
}

bool CmdLine::needProcess() const
{
  return (this->mArguments.length() > 1);
}

CmdLine::ProcessResult CmdLine::process()
{
  ProcessResult result = ProcessResult::None;

  this->mParser.parse(this->mArguments);

  QString modeName = this->mParser.value("mode");
  ModeParserBase *mode = this->createMode(modeName, this->mParser);

  if (mode != nullptr) {
    mode->fillParser();
  }

  this->mParser.process(this->mArguments);

  this->processApplicationOptions();

  if (mode != nullptr) {
    if (mode->collectArguments()) {
      if (mode->process() == 0) {
        result = ProcessResult::Success;
      } else {
        result = ProcessResult::Failed;
      }
    } else {
      qDebug() << "Arguments missing";
      result = ProcessResult::Failed;
    }

    delete mode;

    if (result == ProcessResult::Failed) {
      this->mParser.showHelp(1);
    }
  }

  return result;
}

ModeParserBase *CmdLine::createMode(const QString &name, QCommandLineParser &parser)
{
  if (name == ModeConvertImage::modeName()) {
    return new ModeConvertImage(parser, this);
  }

  if (name == ModeConvertFont::modeName()) {
    return new ModeConvertFont(parser, this);
  }

  if (name == ModeHex2Bin::modeName()) {
    return new ModeHex2Bin(parser, this);
  }

  return nullptr;
}

void CmdLine::addApplicationOptions()
{
  // optional option
  QCommandLineOption optionConfigApp(QStringList() << "config-application",
                                     QCoreApplication::translate("CmdLineParser", "Path to main configuration file. If not specified, default is used."),
                                     QCoreApplication::translate("CmdLineParser", "file"));
  this->mParser.addOption(optionConfigApp);

  // optional option
  QCommandLineOption optionConfigPresets(QStringList() << "config-presets",
                                         QCoreApplication::translate("CmdLineParser", "Path to presets configuration file. If not specified, default is used."),
                                         QCoreApplication::translate("CmdLineParser", "file"));
  this->mParser.addOption(optionConfigPresets);

  // reset all settings
  QCommandLineOption monoOption(QStringList() << "config-initialize",
                                QCoreApplication::translate("CmdLineParser", "Reset all settings to default state."));
  this->mParser.addOption(monoOption);
}

void CmdLine::processApplicationOptions()
{
  // Set path to application configuration file
  if (this->mParser.isSet("config-application")) {
    QString configFile = this->mParser.value("config-application");
    AppSettings::configure(AppSettings::Section::Application, configFile);
  }

  // Set path to presets configuration file
  if (this->mParser.isSet("config-presets")) {
    QString configFile = this->mParser.value("config-presets");
    AppSettings::configure(AppSettings::Section::Presets, configFile);
  }

  // Reset all settings to default
  if (this->mParser.isSet("config-initialize")) {
    AppSettings::reset();
  }
}

}
