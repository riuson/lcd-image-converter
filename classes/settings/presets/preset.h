/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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
class ReorderingOptions;
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
    ReorderingOptions *reordering();
    ImageOptions *image();
    FontOptions *font();
    TemplateOptions *templates();

    static QStringList presetsList();
    static QString selectedName();
    static void setSelectedName(const QString &value);
    static void remove(const QString &value);

    QString name() const;

    bool load(const QString &presetName);
    void save(const QString &presetName) const;
    void saveXML(const QString &filename) const;

    void initMono(MonochromeType type, int edge = 0x80);
    void initGrayscale(int bits);
    void initColor(int redBits, int greenBits, int blueBits);

private:
    PrepareOptions *mPrepare;
    MatrixOptions *mMatrix;
    ReorderingOptions *mReordering;
    ImageOptions *mImage;
    FontOptions *mFont;
    TemplateOptions *mTemplates;
    bool mBlockChangesSignal;
    QString mName;

signals:
    void changed();

public slots:

private slots:
    void partsChanged();

};
//-----------------------------------------------------------------------------
#endif // PRESET_H
