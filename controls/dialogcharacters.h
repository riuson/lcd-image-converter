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

#ifndef DIALOGCHARACTERS_H
#define DIALOGCHARACTERS_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogCharacters;
}
//-----------------------------------------------------------------------------
class CharacterWidget;
//-----------------------------------------------------------------------------
class DialogCharacters : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCharacters(QWidget *parent = 0);
    ~DialogCharacters();
    void selectFont(const QFont &usedfont,
                    const int size,
                    const QString &style,
                    bool monospaced,
                    bool antialiasing);
    void setCharacters(const QString &value);
    void selectedFont(QString *family,
                      QString *style,
                      int *size,
                      bool *monospaced,
                      bool *antialiasing);
    QString selectedCharacters();
public slots:
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
    void insertCharacter(const QString &character);
private:
    Ui::DialogCharacters *ui;
    CharacterWidget *mCharacterWidget;

    QString mCharacters;
    QString mFontFamily;
    QString mStyle;
    int mSize;
    bool mMonospaced;
    bool mAntialiasing;
private slots:
    void on_comboBoxSize_currentIndexChanged(const QString &value);
    void on_comboBoxStyle_currentIndexChanged(const QString &value);
    void on_lineEdit_textChanged(const QString &value);
    void on_radioButtonMonospaced_toggled(bool value);
    void on_checkBoxAntialiasing_toggled(bool value);
};
//-----------------------------------------------------------------------------
#endif // DIALOGCHARACTERS_H
