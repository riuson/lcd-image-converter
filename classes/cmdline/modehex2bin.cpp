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

#include "modehex2bin.h"
#include "datacontainer.h"
#include <QCommandLineParser>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>

namespace CommandLine
{

ModeHex2Bin::ModeHex2Bin(QCommandLineParser *parser, QObject *parent) :
  ModeParserBase(parser, parent)
{
}

QString ModeHex2Bin::modeName()
{
  return "hex2bin";
}

void ModeHex2Bin::fillParser() const
{
  // --input=/temp/1.c
  QCommandLineOption inputOption(QStringList() << "i" << "input",
                                 QCoreApplication::translate("CmdLineParser", "Full <path> to image in *.C format."),
                                 QCoreApplication::translate("CmdLineParser", "path"));
  this->mParser->addOption(inputOption);

  // --output=/temp/1.bin
  QCommandLineOption outputOption(QStringList() << "o" << "output",
                                  QCoreApplication::translate("CmdLineParser", "Full <path> to output binary result."),
                                  QCoreApplication::translate("CmdLineParser", "path"));
  this->mParser->addOption(outputOption);
}

bool ModeHex2Bin::collectArguments()
{
  this->mInputFilename = this->mParser->value("input");
  this->mOuputFilename = this->mParser->value("output");

  return (!this->mInputFilename.isEmpty() &&
          !this->mOuputFilename.isEmpty());
}

int ModeHex2Bin::process()
{
  // check input file exists
  if (QFile::exists(this->mInputFilename)) {
    // check output file exists
    //if (QFile::exists(this->mOuputFilename))
    {
      QFile inputFile(this->mInputFilename);

      if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream inputStream(&inputFile);
        QString inputText = inputStream.readAll();
        inputFile.close();

        QByteArray data;
        data = this->hex2bin(inputText);

        // save to output file
        QFile outputFile(this->mOuputFilename);

        if (outputFile.open(QFile::WriteOnly)) {
          outputFile.write(data);
          outputFile.close();
        }
      }

      return 0;
    }
  }

  return 1;
}

QByteArray ModeHex2Bin::hex2bin(QString &hexString)
{
  QByteArray result;

  int size = 1;
  bool le = true;

  QTextStream stream(&hexString, QIODevice::ReadOnly);
  QString part;

  while (!stream.atEnd()) {
    stream >> part;

    if (part == "uint32") {
      size = 4;
      continue;
    } else if (part == "uint24") {
      size = 3;
      continue;
    } else if (part == "uint16") {
      size = 2;
      continue;
    } else if (part == "uint8") {
      size = 1;
      continue;
    }

    if (part == "le") {
      le = true;
      continue;
    } else if (part == "be") {
      le = false;
      continue;
    }

    bool ok;
    quint32 value = part.toUInt(&ok, 10);

    if (ok) { // decimal
      if (le) {
        this->appendDataLE(&result, size, value);
      } else {
        this->appendDataBE(&result, size, value);
      }
    } else {
      value = part.toUInt(&ok, 16);

      if (ok) {
        if (le) {
          this->appendDataLE(&result, size, value);
        } else {
          this->appendDataBE(&result, size, value);
        }
      }
    }
  }

  return result;
}

void ModeHex2Bin::appendDataLE(QByteArray *array, int size, quint32 value)
{
  while (size -- > 0) {
    quint8 a = value & 0xff;
    array->append(a);
    value = value >> 8;
  }
}

void ModeHex2Bin::appendDataBE(QByteArray *array, int size, quint32 value)
{
  int index = array->length();

  while (size -- > 0) {
    quint8 a = value & 0xff;
    array->insert(index, a);
    value = value >> 8;
  }
}

}
