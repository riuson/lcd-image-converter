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

#ifndef PREPAREOPTIONS_H
#define PREPAREOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class QSettings;
class QDomElement;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class PrepareOptions : public QObject
{
    Q_OBJECT
public:
    explicit PrepareOptions(QObject *parent = 0);

    ConversionType convType() const;
    MonochromeType monoType() const;
    int edge() const;
    ScanMainDirection scanMain() const;
    ScanSubDirection scanSub() const;
    bool inverse() const;
    bool bandScanning() const;
    int bandWidth() const;
    bool useCustomScript() const;
    QString customScript() const;

    void setConvType(ConversionType value);
    void setMonoType(MonochromeType value);
    void setEdge(int value);
    void setScanMain(ScanMainDirection value);
    void setScanSub(ScanSubDirection value);
    void setInverse(bool value);
    void setBandScanning(bool value);
    void setBandWidth(int value);
    void setUseCustomScript(bool value);
    void setCustomScript(const QString &value);

    const QString & convTypeName() const;
    const QString & monoTypeName() const;

    bool load(QSettings *settings, int version);
    bool loadXmlElement(QDomElement *element);
    void save(QSettings *settings);
    void saveXmlElement(QDomElement *element);

private:
    static const QString GroupName;
    static const QString FieldConvType;
    static const QString FieldMonoType;
    static const QString FieldEdge;
    static const QString FieldScanMain;
    static const QString FieldScanSub;
    static const QString FieldInverse;
    static const QString FieldBandScanning;
    static const QString FieldBandWidth;
    static const QString FieldUseCustomScript;
    static const QString FieldCustomScript;

    ConversionType mConvType;
    MonochromeType mMonoType;
    int mEdge;
    ScanMainDirection mScanMain;
    ScanSubDirection mScanSub;
    bool mInverse;
    bool mBandScanning;
    int mBandWidth;
    bool mUseCustomScript;
    QString mCustomScript;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // PREPAREOPTIONS_H
