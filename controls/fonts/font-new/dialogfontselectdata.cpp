/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#include "dialogfontselectdata.h"

#include <QTableWidgetSelectionRange>
#include <QColorDialog>
#include <QFontDatabase>
#include "charactersmodel.h"
#include "unicodeblocksmodel.h"
#include "unicodeblocksfiltermodel.h"
#include "dialogfontrange.h"
#include "fonthelper.h"
#include "fonteditoroptions.h"
#include "tfontparameters.h"
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
DialogFontSelectData::DialogFontSelectData(QObject *parent) :
    QObject(parent)
{
    this->mModel = new CharactersModel(this);
    this->mModel->setCodesRange(0x0000, 0x00ff);

    this->mSize = 14;
    this->mMonospaced = false;
    this->mAntialiasing = false;
    this->mAlphaChannel = false;

    QString defChars;

    for (int i = 0x20; i < 0x7f; i++)
    {
        defChars += QString(QChar(i));
    }

    this->mCharacters = defChars;

    this->mBlocksModel = new UnicodeBlocksModel(this);
    this->mBlocksFilterModel = new UnicodeBlocksFilterModel(this);
    this->mBlocksFilterModel->setSourceModel(this->mBlocksModel);

    this->mSortOrderUp = false;

    QPixmap pixmapForeColor = QPixmap(24, 24);
    pixmapForeColor.fill(FontEditorOptions::foreColor());

    QPixmap pixmapBackColor = QPixmap(24, 24);
    pixmapBackColor.fill(FontEditorOptions::backColor());
}
//-----------------------------------------------------------------------------
DialogFontSelectData::~DialogFontSelectData()
{
}
//-----------------------------------------------------------------------------
QString DialogFontSelectData::characters()
{
    return this->mCharacters;
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::getFontParameters(tFontParameters *parameters)
{
    parameters->family = this->mFontFamily;
    parameters->style = this->mFontStyle;
    parameters->size = this->mSize;
    parameters->monospaced = this->mMonospaced;
    parameters->antiAliasing = this->mAntialiasing;
    parameters->alphaChannel = this->mAlphaChannel;

    // ascent/descent
    {
        QFontDatabase fonts;
        QFont font = fonts.font(parameters->family, parameters->style, parameters->size);
        QFontMetrics metrics(font);
        parameters->ascent = metrics.ascent();
        parameters->descent = metrics.descent();
    }

}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setCharacters(const QString &value)
{
    this->mCharacters = value;
    emit this->charactersListChanged(this->mCharacters);
    this->notifyFontChanged();
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setFontParameters(const tFontParameters &parameters)
{
    this->mFontFamily = parameters.family;
    this->mFontStyle = parameters.style;
    this->mSize = parameters.size;

    this->mAlphaChannel = parameters.alphaChannel;
    this->mAntialiasing = parameters.antiAliasing;
    this->mMonospaced = parameters.monospaced;

    this->notifyFontChanged();

    emit this->alphaChannelChanged(this->mAlphaChannel);
    emit this->antialiasingChanged(this->mAntialiasing);
    emit this->monospacedChanged(this->mMonospaced);
}
//-----------------------------------------------------------------------------
CharactersModel *DialogFontSelectData::charactersModel()
{
    return this->mModel;
}
//-----------------------------------------------------------------------------
UnicodeBlocksFilterModel *DialogFontSelectData::unicodeBlocksModel()
{
    return this->mBlocksFilterModel;
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::notifyFontChanged()
{
    QFontDatabase fonts;
    QFont font = fonts.font(this->mFontFamily, this->mFontStyle, this->mSize);
    font.setPixelSize(this->mSize);
    emit this->fontChanged(font);


    // find max size
    QFontMetrics metrics(font);
    QSize sz = QSize();
    QString chars = this->characters();

    for (int i = 0; i < chars.count(); i++)
    {
        QSize sz1 = FontHelper::getCharacterSize(metrics, chars.at(i));
        sz.setWidth(qMax(sz.width(), sz1.width()));
        sz.setHeight(qMax(sz.height(), sz1.height()));
    }

    emit this->fontMeasured(chars.count(), sz.width(), sz.height());
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setFont(const QFont &font)
{
    this->mFontFamily = font.family();

    QString style = this->mFontStyle;

    QFontDatabase fonts;
    QStringList stylesList = fonts.styles(this->mFontFamily);
    emit this->stylesListChanged(stylesList, style);
    this->notifyFontChanged();
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setStyle(const QString &style)
{
    this->mFontStyle = style;

    int previousSize = this->mSize;

    QFontDatabase fonts;

    QList<int> sizes;

    if (fonts.isSmoothlyScalable(this->mFontFamily, this->mFontStyle))
        sizes = fonts.smoothSizes(this->mFontFamily, this->mFontStyle);
    else
        sizes = fonts.standardSizes();

    emit this->sizesListChanged(sizes, previousSize);
    this->notifyFontChanged();
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setSize(const QString &text)
{
    bool ok;
    int a = text.toInt(&ok);
    if (ok)
        this->mSize = a;
    else
        this->mSize = 5;

    this->notifyFontChanged();
    this->notifyFontChanged();
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setUnicodeBlocksFilter(const QString &text)
{
    this->mBlocksFilterModel->setNameFilter(text);
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setUnicodeRange(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();

    if (indexes.length() > 0)
    {
        QAbstractItemModel *model = this->unicodeBlocksModel();
        bool ok;
        quint32 first = model->data(indexes.at(0), UnicodeBlocksModel::FirstCodeRole).toUInt(&ok);
        quint32 last = model->data(indexes.at(0), UnicodeBlocksModel::LastCodeRole).toUInt(&ok);

        this->mModel->setCodesRange(first, last);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::appendCharacters(const QString &value)
{
    QString result = this->characters();

    for (int i = 0; i < value.length(); i++)
    {
        QChar c = value.at(i);

        if (!result.contains(c))
        {
            result.append(c);
        }
    }

    this->setCharacters(result);
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setMonospaced(bool value)
{
    this->mMonospaced = value;
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setAntialiasing(bool value)
{
    this->mAntialiasing = value;
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::setAlphaChannel(bool value)
{
    this->mAlphaChannel = value;
}
//-----------------------------------------------------------------------------
void DialogFontSelectData::resort()
{
    QString chars = this->characters();
    QList<QChar> list;

    for (int i = 0; i < chars.length(); i++)
    {
        list.append(chars.at(i));
    }

    qSort(list);
    chars = QString();

    if (!this->mSortOrderUp)
    {
        for (int i = 0; i < list.length(); i++)
        {
            chars.append(list.at(i));
        }
    }
    else
    {
        for (int i = list.length() - 1; i >= 0; i--)
        {
            chars.append(list.at(i));
        }
    }

    this->mSortOrderUp = !this->mSortOrderUp;
    this->setCharacters(chars);
}
//-----------------------------------------------------------------------------
