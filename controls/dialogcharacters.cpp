/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "dialogcharacters.h"
#include "ui_dialogcharacters.h"

#include "widgetcharacters.h"
//-----------------------------------------------------------------------------
DialogCharacters::DialogCharacters(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogCharacters)
{
    ui->setupUi(this);
    this->mCharacterWidget = new CharacterWidget(this);
    this->ui->scrollArea->setWidget(this->mCharacterWidget);

    this->findStyles(this->ui->comboBoxFont->currentFont());
    this->findSizes(this->ui->comboBoxFont->currentFont());

    this->connect(this->ui->comboBoxFont, SIGNAL(currentFontChanged(QFont)),
                  this->mCharacterWidget, SLOT(updateFont(QFont)));
    this->connect(this->ui->comboBoxSize, SIGNAL(currentIndexChanged(QString)),
                  this->mCharacterWidget, SLOT(updateSize(QString)));
    this->connect(this->ui->comboBoxStyle, SIGNAL(currentIndexChanged(QString)),
                  this->mCharacterWidget, SLOT(updateStyle(QString)));
    this->connect(this->mCharacterWidget, SIGNAL(characterSelected(QString)),
                  this, SLOT(insertCharacter(QString)));
    this->connect(this->ui->checkBoxFontMerging, SIGNAL(toggled(bool)),
                  this->mCharacterWidget, SLOT(updateFontMerging(bool)));

    this->mCharacterWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QString defChars;
    for (int i = 0x20; i < 0x7f; i++)
    {
        defChars += QString(QChar(i));
    }
    this->ui->lineEdit->setText(defChars);

    this->mAntialiasing = false;
    this->mMonospaced = false;
}
//-----------------------------------------------------------------------------
DialogCharacters::~DialogCharacters()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogCharacters::selectFont(const QFont &usedfont,
                                  const int size,
                                  const QString &style,
                                  bool monospaced,
                                  bool antialiasing)
{
    this->ui->comboBoxFont->setCurrentFont(usedfont);

    int indexSize = this->ui->comboBoxSize->findText(QString("%1").arg(size));
    if (indexSize >= 0)
        this->ui->comboBoxSize->setCurrentIndex(indexSize);

    int indexStyle = this->ui->comboBoxStyle->findText(style);
    if (indexStyle >= 0)
        this->ui->comboBoxStyle->setCurrentIndex(indexStyle);
    this->ui->checkBoxAntialiasing->setChecked(antialiasing);
    this->ui->radioButtonMonospaced->setChecked(monospaced);
}
//-----------------------------------------------------------------------------
void DialogCharacters::setCharacters(const QString &value)
{
    this->ui->lineEdit->setText(value);
}
//-----------------------------------------------------------------------------
void DialogCharacters::selectedFont(QString *family,
                                    QString *style,
                                    int *size,
                                    bool *monospaced,
                                    bool *antialiasing)
{
    *family = this->mFontFamily;
    *style = this->mStyle;
    *size = this->mSize;
    *monospaced = this->mMonospaced;
    *antialiasing = this->mAntialiasing;
}
//-----------------------------------------------------------------------------
QString DialogCharacters::selectedCharacters()
{
    return this->mCharacters;
}
//-----------------------------------------------------------------------------
void DialogCharacters::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentItem = this->ui->comboBoxStyle->currentText();
    this->ui->comboBoxStyle->clear();

    QString style;
    foreach (style, fontDatabase.styles(font.family()))
        this->ui->comboBoxStyle->addItem(style);

    int styleIndex = this->ui->comboBoxStyle->findText(currentItem);

    if (styleIndex == -1)
        this->ui->comboBoxStyle->setCurrentIndex(0);
    else
        this->ui->comboBoxStyle->setCurrentIndex(styleIndex);

    this->mFontFamily = font.family();
}
//-----------------------------------------------------------------------------
void DialogCharacters::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = this->ui->comboBoxSize->currentText();
    this->ui->comboBoxSize->blockSignals(true);
    this->ui->comboBoxSize->clear();

    int size;
    if(fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font))) {
        foreach(size, QFontDatabase::standardSizes()) {
            this->ui->comboBoxSize->addItem(QVariant(size).toString());
            this->ui->comboBoxSize->setEditable(true);
        }

    } else {
        foreach(size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font))) {
            this->ui->comboBoxSize->addItem(QVariant(size).toString());
            this->ui->comboBoxSize->setEditable(false);
        }
    }

    this->ui->comboBoxSize->blockSignals(false);

    int sizeIndex = this->ui->comboBoxSize->findText(currentSize);

    if(sizeIndex == -1)
        this->ui->comboBoxSize->setCurrentIndex(qMax(0, this->ui->comboBoxSize->count() / 3));
    else
        this->ui->comboBoxSize->setCurrentIndex(sizeIndex);
}
//-----------------------------------------------------------------------------
void DialogCharacters::insertCharacter(const QString &character)
{
    this->ui->lineEdit->insert(character);
}
//-----------------------------------------------------------------------------
void DialogCharacters::on_comboBoxSize_currentIndexChanged(const QString &value)
{
    bool ok;
    int result = value.toInt(&ok);
    if (ok)
        this->mSize = result;
}
//-----------------------------------------------------------------------------
void DialogCharacters::on_comboBoxStyle_currentIndexChanged(const QString &value)
{
    if (!value.isEmpty())
        this->mStyle = value;
}
//-----------------------------------------------------------------------------
void DialogCharacters::on_lineEdit_textChanged(const QString &value)
{
    this->mCharacters = value;
}
//-----------------------------------------------------------------------------
void DialogCharacters::on_radioButtonMonospaced_toggled(bool value)
{
    this->mMonospaced = value;
}
//-----------------------------------------------------------------------------
void DialogCharacters::on_checkBoxAntialiasing_toggled(bool value)
{
    this->mAntialiasing = value;
}
//-----------------------------------------------------------------------------
