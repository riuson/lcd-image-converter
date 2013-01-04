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

#include "preset.h"
//-----------------------------------------------------------------------------
#include <QStringList>
#include <QSettings>
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "reorderingoptions.h"
#include "imageoptions.h"
#include "fontoptions.h"
#include "templateoptions.h"
//-----------------------------------------------------------------------------
Preset::Preset(QObject *parent) :
    QObject(parent)
{
    this->mBlockChangesSignal = false;

    this->mPrepare    = new PrepareOptions(this);
    this->mMatrix     = new MatrixOptions(this);
    this->mReordering = new ReorderingOptions(this);
    this->mImage      = new ImageOptions(this);
    this->mFont       = new FontOptions(this);
    this->mTemplates  = new TemplateOptions(this);

    this->connect(this->mPrepare,    SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mMatrix,     SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mReordering, SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mImage,      SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mFont,       SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mTemplates,  SIGNAL(changed()), SLOT(partsChanged()));
}
//-----------------------------------------------------------------------------
Preset::~Preset()
{
    delete this->mTemplates;
    delete this->mFont;
    delete this->mImage;
    delete this->mReordering;
    delete this->mMatrix;
    delete this->mPrepare;
}
//-----------------------------------------------------------------------------
PrepareOptions *Preset::prepare()
{
    return this->mPrepare;
}
//-----------------------------------------------------------------------------
MatrixOptions *Preset::matrix()
{
    return this->mMatrix;
}
//-----------------------------------------------------------------------------
ReorderingOptions *Preset::reordering()
{
    return this->mReordering;
}
//-----------------------------------------------------------------------------
ImageOptions *Preset::image()
{
    return this->mImage;
}
//-----------------------------------------------------------------------------
FontOptions *Preset::font()
{
    return this->mFont;
}
//-----------------------------------------------------------------------------
TemplateOptions *Preset::templates()
{
    return this->mTemplates;
}
//-----------------------------------------------------------------------------
QStringList Preset::presetsList()
{
    QSettings sett;
    sett.beginGroup("presets");
    QStringList names = sett.childGroups();
    sett.endGroup();

    return names;
}
//-----------------------------------------------------------------------------
QString Preset::currentName()
{
    QSettings sett;
    sett.beginGroup("presets");
    QString result = sett.value("selected", QVariant("")).toString();
    sett.endGroup();

    return result;
}
//-----------------------------------------------------------------------------
void Preset::setCurrentName(const QString &value)
{
    QSettings sett;
    sett.beginGroup("presets");
    sett.setValue("selected", QVariant(value));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void Preset::remove(const QString &value)
{
    QSettings sett;
    sett.beginGroup("presets");

    sett.beginGroup(value);
    sett.remove("");

    sett.endGroup();
}
//-----------------------------------------------------------------------------
bool Preset::load(const QString &name)
{
    bool result = false;

    if (!name.isEmpty())
    {
        this->mBlockChangesSignal = true;

        QSettings sett;
        sett.beginGroup("presets");

        if (sett.childGroups().contains(name))
        {
            sett.beginGroup(name);

            // get version of settings
            int version;
            bool ok;
            QVariant varVersion = sett.value("version", QVariant((int)1));
            version = varVersion.toInt(&ok);

            if (!ok)
                version = 1;

            result = this->mPrepare->load(&sett, version);
            result &= this->mMatrix->load(&sett, version);
            result &= this->mReordering->load(&sett, version);
            result &= this->mImage->load(&sett, version);
            result &= this->mFont->load(&sett, version);
            result &= this->mTemplates->load(&sett, version);

            sett.endGroup();
        }
        sett.endGroup();

        this->mBlockChangesSignal = false;

        emit this->changed();
    }
    return result;
}
//-----------------------------------------------------------------------------
void Preset::save(const QString &name) const
{
    QSettings sett;
    sett.beginGroup("presets");

    sett.beginGroup(name);
    sett.remove("");

    sett.setValue("version", (int)2);

    this->mPrepare->save(&sett);
    this->mMatrix->save(&sett);
    this->mReordering->save(&sett);
    this->mImage->save(&sett);
    this->mFont->save(&sett);
    this->mTemplates->save(&sett);

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void Preset::initMono(MonochromeType type, int edge)
{
    this->mMatrix->operationsRemoveAll();
    this->mReordering->operationsRemoveAll();

    this->mPrepare->setConvType(ConversionTypeMonochrome);
    this->mPrepare->setMonoType(type);
    this->mPrepare->setEdge(edge);
    this->mMatrix->setMaskUsed(0x00000001);
    this->mMatrix->setMaskAnd(0xffffffff);
    this->mMatrix->setMaskOr(0x00000000);
    this->mMatrix->setMaskFill(0x000000ff);
    this->mImage->setBlockSize(Data8);

    // bits shift
    {
        this->mMatrix->operationAdd(0x00000001, 0, false);
    }

    emit this->changed();
}
//-----------------------------------------------------------------------------
void Preset::initGrayscale(int bits)
{
    this->mMatrix->operationsRemoveAll();
    this->mReordering->operationsRemoveAll();

    if (bits > 8) bits = 8;
    if (bits < 1) bits = 1;

    this->mPrepare->setConvType(ConversionTypeGrayscale);
    this->mImage->setBlockSize(Data16);

    // mask of used bits before packing
    {
        quint32 mask = 0x000000ff;
        mask = mask << bits;
        mask = mask >> 8;
        mask = mask & 0x000000ff;

        this->mMatrix->setMaskUsed(mask);
    }

    this->mMatrix->setMaskAnd(0xffffffff);
    this->mMatrix->setMaskOr(0x00000000);
    this->mMatrix->setMaskFill(0x0000ffff);

    // bits shift
    {
        quint32 mask = 0x0000ff00;
        mask = mask >> bits;
        mask = mask & 0x000000ff;

        quint32 shift = 8 - bits;

        this->mMatrix->operationAdd(mask, shift, false);
    }

    emit this->changed();
}
//-----------------------------------------------------------------------------
void Preset::initColor(int redBits, int greenBits, int blueBits)
{
    this->mMatrix->operationsRemoveAll();
    this->mReordering->operationsRemoveAll();

    if (redBits > 8) redBits = 8;
    if (redBits < 1) redBits = 1;
    if (greenBits > 8) greenBits = 8;
    if (greenBits < 1) greenBits = 1;
    if (blueBits > 8) blueBits = 8;
    if (blueBits < 1) blueBits = 1;

    int bits = redBits + greenBits + blueBits;

    this->mPrepare->setConvType(ConversionTypeColor);
    this->mImage->setBlockSize(Data32);

    // mask of used bits before packing
    {
        quint64 mask64 = 0x00000000ffffffff;
        mask64 = mask64 << bits;
        mask64 = mask64 >> 32;
        mask64 = mask64 & 0x0000000000ffffff; // 24 bits
        quint32 mask = (quint32)mask64;
        this->mMatrix->setMaskUsed(mask);
    }

    this->mMatrix->setMaskAnd(0xffffffff);
    this->mMatrix->setMaskOr(0x00000000);
    this->mMatrix->setMaskFill(0xffffffff);

    // red bits shift
    {
        quint32 mask = 0x0000ff00;
        mask = mask >> redBits;
        mask = mask & 0x000000ff;
        mask = mask << 16;

        quint32 shift = 24 - bits;

        this->mMatrix->operationAdd(mask, shift, false);
    }

    // green bits shift
    {
        quint32 mask = 0x0000ff00;
        mask = mask >> greenBits;
        mask = mask & 0x000000ff;
        mask = mask << 8;

        quint32 shift = 16 - greenBits - blueBits;

        this->mMatrix->operationAdd(mask, shift, false);
    }

    // blue bits shift
    {
        quint32 mask = 0x0000ff00;
        mask = mask >> blueBits;
        mask = mask & 0x000000ff;

        quint32 shift = 8 - blueBits;

        this->mMatrix->operationAdd(mask, shift, false);
    }

    emit this->changed();
}
//-----------------------------------------------------------------------------
void Preset::partsChanged()
{
    if (!this->mBlockChangesSignal)
        emit this->changed();
}
//-----------------------------------------------------------------------------
