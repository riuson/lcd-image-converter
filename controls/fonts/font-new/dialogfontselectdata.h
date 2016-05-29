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

#ifndef DIALOGFONTSELECTDATA_H
#define DIALOGFONTSELECTDATA_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class CharactersModel;
class QModelIndex;
class QItemSelection;
class UnicodeBlocksModel;
class UnicodeBlocksFilterModel;
struct tFontParameters;
//-----------------------------------------------------------------------------
class DialogFontSelectData : public QObject
{
    Q_OBJECT
public:
    explicit DialogFontSelectData(QObject *parent = 0);
    virtual ~DialogFontSelectData();

    QString characters();
    void getFontParameters(tFontParameters *parameters);

    void setCharacters(const QString &value);
    void setFontParameters(const tFontParameters &parameters);

    CharactersModel *charactersModel();
    UnicodeBlocksFilterModel *unicodeBlocksModel();

    void appendCharacters(const QString &value);
private:
    CharactersModel *mModel;
    UnicodeBlocksModel *mBlocksModel;
    UnicodeBlocksFilterModel *mBlocksFilterModel;
    bool mSortOrderUp;

    QString mFontFamily;
    QString mFontStyle;
    int mSize;
    bool mMonospaced;
    bool mAntialiasing;
    bool mAlphaChannel;
    QString mCharacters;

    void notifyFontChanged();

signals:
    void stylesListChanged(const QStringList &list, const QString &selected);
    void sizesListChanged(const QList<int> &list, int selected);
    void charactersListChanged(const QString &value);
    void fontChanged(const QFont &value);
    void monospacedChanged(bool value);
    void antialiasingChanged(bool value);
    void alphaChannelChanged(bool value);

public slots:
    void setFont(const QFont &font);
    void setStyle(const QString &style);
    void setSize(const QString &text);
    void setUnicodeBlocksFilter(const QString & text);
    void setUnicodeRange(const QItemSelection &selected, const QItemSelection &deselected);
    void setMonospaced(bool value);
    void setAntialiasing(bool value);
    void setAlphaChannel(bool value);
    void resort();
};
//-----------------------------------------------------------------------------
#endif // DIALOGFONTSELECTDATA_H
