#include "convertermono.h"

#include <QSettings>
//-----------------------------------------------------------------------------
ConverterMono::ConverterMono(QObject *parent) :
        QObject(parent)
{
    this->mBytesOrder = BigEndian;
    this->mDataLength = Data8;
    this->mMirrorBytes = false;
    this->mBlackWhiteLevel = 128;
}
//-----------------------------------------------------------------------------
ConverterMono::~ConverterMono()
{
}
//-----------------------------------------------------------------------------
void ConverterMono::loadSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("mono");

    bool ok;
    int a = sett.value("endian", QVariant(BigEndian)).toInt(&ok);
    if (ok)
        this->mBytesOrder = (BytesOrder)a;

    a = sett.value("dataLength", QVariant(Data8)).toInt(&ok);
    if (ok)
        this->mDataLength = (DataLength)a;

    this->mMirrorBytes = sett.value("mirror", QVariant(false)).toBool();

    a = sett.value("level", QVariant(128)).toInt(&ok);
    if (ok)
        this->mBlackWhiteLevel = a;

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ConverterMono::saveSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("mono");

    sett.setValue("endian", QVariant(this->mBytesOrder));

    sett.setValue("dataLength", QVariant(this->mDataLength));

    sett.setValue("mirror", QVariant(this->mMirrorBytes));

    sett.setValue("level", QVariant(this->mBlackWhiteLevel));

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
QString ConverterMono::name()
{
    return "mono";
}
//-----------------------------------------------------------------------------
QString ConverterMono::displayName()
{
    return tr("Monochrome");
}
//-----------------------------------------------------------------------------
void ConverterMono::options(BytesOrder *orderBytes,
                            DataLength *length,
                            bool *mirror,
                            int *level)
{
    *orderBytes = this->mBytesOrder;
    *length = this->mDataLength;
    *mirror = this->mMirrorBytes;
    *level = this->mBlackWhiteLevel;
}
//-----------------------------------------------------------------------------
void ConverterMono::setOptions(const BytesOrder &orderBytes,
                               const DataLength &length,
                               const bool mirror,
                               const int level)
{
    this->mBytesOrder = orderBytes;
    this->mDataLength = length;
    this->mMirrorBytes = mirror;
    this->mBlackWhiteLevel = level;
}
//-----------------------------------------------------------------------------
