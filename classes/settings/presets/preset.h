/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#ifndef PRESET_H
#define PRESET_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class PrepareOptions;
class MatrixOptions;
class ImageOptions;
class FontOptions;
class TemplateOptions;
class QString;
class QStringList;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class Preset : public QObject
{
    Q_OBJECT
public:
    explicit Preset(QObject *parent = 0);
    virtual ~Preset();

    PrepareOptions *prepare();
    MatrixOptions *matrix();
    ImageOptions *image();
    FontOptions *font();
    TemplateOptions *templates();

    static QStringList presetsList();
    static QString currentName();
    static void setCurrentName(const QString &value);
    static void remove(const QString &value);

    bool load(const QString &name);
    void save(const QString &name) const;

    void initMono(MonochromeType type, int edge = 0x80);
    void initGrayscale(int bits);
    void initColor(int redBits, int greenBits, int blueBits);

private:
    PrepareOptions *mPrepare;
    MatrixOptions *mMatrix;
    ImageOptions *mImage;
    FontOptions *mFont;
    TemplateOptions *mTemplates;
    bool mBlockChangesSignal;

signals:
    void changed();

public slots:

private slots:
    void partsChanged();

};
//-----------------------------------------------------------------------------
#endif // PRESET_H
