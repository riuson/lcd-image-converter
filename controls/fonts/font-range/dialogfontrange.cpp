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

#include "dialogfontrange.h"
#include "ui_dialogfontrange.h"

#include "fontoptions.h"
#include "fonthelper.h"
#include <QTextCodec>
#include <QCompleter>

DialogFontRange::DialogFontRange(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogFontRange)
{
  ui->setupUi(this);

  this->connect(this->ui->comboBoxEncoding, SIGNAL(currentIndexChanged(QString)), SLOT(updatePreview()));
  this->connect(this->ui->spinBoxFrom, SIGNAL(valueChanged(int)), SLOT(updatePreview()));
  this->connect(this->ui->spinBoxTo, SIGNAL(valueChanged(int)), SLOT(updatePreview()));
  this->connect(this->ui->radioButtonBigEndian, SIGNAL(toggled(bool)), SLOT(updatePreview()));
  this->connect(this->ui->radioButtonLittleEndian, SIGNAL(toggled(bool)), SLOT(updatePreview()));

  this->mResultString = QString();

  QStringList encodings = FontOptions::encodings();
  qSort(encodings);
  this->ui->comboBoxEncoding->addItems(encodings);
  int index = this->ui->comboBoxEncoding->findText("UTF-8", Qt::MatchFixedString);

  if (index >= 0) {
    this->ui->comboBoxEncoding->setCurrentIndex(index);
  }

  this->mEncodingCompleter = new QCompleter(FontOptions::encodings(), this);
  this->mEncodingCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  this->mEncodingCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  this->mEncodingCompleter->setFilterMode(Qt::MatchContains);
  this->ui->comboBoxEncoding->setCompleter(this->mEncodingCompleter);
}

DialogFontRange::~DialogFontRange()
{
  delete ui;
}

const QString &DialogFontRange::resultString() const
{
  return this->mResultString;
}

void DialogFontRange::updatePreview()
{
  QString encoding = this->ui->comboBoxEncoding->currentText();
  int from = this->ui->spinBoxFrom->value();
  int to = this->ui->spinBoxTo->value();
  bool bigEndian = this->ui->radioButtonBigEndian->isChecked();
  this->updatePreview(encoding, from, to, bigEndian);
}

void DialogFontRange::updatePreview(const QString &encoding, int from, int to, bool bigEndian)
{
  QTextCodec *codec = QTextCodec::codecForName(encoding.toLatin1());
  QString result;

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

      QString str = codec->toUnicode(array);
      result += str;
    } else {
      result += QChar(QChar::Null);
    }
  }

  this->ui->plainTextEditPreview->setPlainText(FontHelper::escapeControlChars(result));
}

void DialogFontRange::on_plainTextEditPreview_textChanged()
{
  this->mResultString = FontHelper::unescapeControlChars(this->ui->plainTextEditPreview->toPlainText());
}

