#include "convertercolor.h"

#include <QSettings>
#include <QImage>
#include <QPainter>
#include <QColor>
//-----------------------------------------------------------------------------
ConverterColor::ConverterColor(QObject *parent) :
        QObject(parent)
{
    this->mBytesOrder = BigEndian;
    this->mDataLength = Data8;
    this->mMirrorBytes = false;
    this->mPack = true;
    this->mBitsPerPointRed = 3;
    this->mBitsPerPointGreen = 2;
    this->mBitsPerPointBlue = 3;
}
//-----------------------------------------------------------------------------
ConverterColor::~ConverterColor()
{
}
//-----------------------------------------------------------------------------
void ConverterColor::loadSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("color");

    bool ok;
    int a = sett.value("endian", QVariant(BigEndian)).toInt(&ok);
    if (ok)
        this->mBytesOrder = (BytesOrder)a;

    a = sett.value("dataLength", QVariant(Data8)).toInt(&ok);
    if (ok)
        this->mDataLength = (DataLength)a;

    this->mMirrorBytes = sett.value("mirror", QVariant(false)).toBool();
    this->mPack = sett.value("pack", QVariant(true)).toBool();

    a = sett.value("bitsPerPointRed", QVariant(3)).toInt(&ok);
    if (ok)
        this->mBitsPerPointRed = a;

    a = sett.value("bitsPerPointGreen", QVariant(2)).toInt(&ok);
    if (ok)
        this->mBitsPerPointGreen = a;

    a = sett.value("bitsPerPointBlue", QVariant(3)).toInt(&ok);
    if (ok)
        this->mBitsPerPointBlue = a;

    a = sett.value("colorsOrder", QVariant(ColorsOrderRGB)).toInt(&ok);
    if (ok)
        this->mOrderColors = (ColorsOrder)a;

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ConverterColor::saveSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("color");

    sett.setValue("endian", QVariant(this->mBytesOrder));

    sett.setValue("dataLength", QVariant(this->mDataLength));

    sett.setValue("mirror", QVariant(this->mMirrorBytes));
    sett.setValue("pack", QVariant(this->mPack));

    sett.setValue("bitsPerPointRed", QVariant(this->mBitsPerPointRed));
    sett.setValue("bitsPerPointGreen", QVariant(this->mBitsPerPointGreen));
    sett.setValue("bitsPerPointBlue", QVariant(this->mBitsPerPointBlue));

    sett.setValue("colorsOrder", QVariant(this->mOrderColors));

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
QString ConverterColor::name()
{
    return "color";
}
//-----------------------------------------------------------------------------
QString ConverterColor::displayName()
{
    return tr("Color");
}
//-----------------------------------------------------------------------------
QImage ConverterColor::preprocessImage(const QImage &source)
{
    QImage result(source);
    this->makeGradations(result);
    return result;
}
//-----------------------------------------------------------------------------
void ConverterColor::options(BytesOrder *orderBytes,
                            DataLength *length,
                            bool *mirror,
                            bool *pack,
                            int *bitsPerPointRed,
                            int *bitsPerPointGreen,
                            int *bitsPerPointBlue,
                            ColorsOrder *orderColors)
{
    *orderBytes = this->mBytesOrder;
    *length = this->mDataLength;
    *mirror = this->mMirrorBytes;
    *pack = this->mPack;
    *bitsPerPointRed = this->mBitsPerPointRed;
    *bitsPerPointGreen = this->mBitsPerPointGreen;
    *bitsPerPointBlue = this->mBitsPerPointBlue;
    *orderColors = this->mOrderColors;
}
//-----------------------------------------------------------------------------
void ConverterColor::setOptions(const BytesOrder &orderBytes,
                               const DataLength &length,
                               const bool mirror,
                               const bool pack,
                               const int bitsPerPointRed,
                               const int bitsPerPointGreen,
                               const int bitsPerPointBlue,
                               const ColorsOrder orderColors)
{
    this->mBytesOrder = orderBytes;
    this->mDataLength = length;
    this->mMirrorBytes = mirror;
    this->mPack = pack;
    this->mBitsPerPointRed = bitsPerPointRed;
    this->mBitsPerPointGreen = bitsPerPointGreen;
    this->mBitsPerPointBlue = bitsPerPointBlue;
    this->mOrderColors = orderColors;
}
//-----------------------------------------------------------------------------
void ConverterColor::makeGradations(QImage &image)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, false);
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            QRgb value = image.pixel(x, y);
            int r = qRed  (value) & (0x0000ff00 >> this->mBitsPerPointRed);
            int g = qGreen(value) & (0x0000ff00 >> this->mBitsPerPointGreen);
            int b = qBlue (value) & (0x0000ff00 >> this->mBitsPerPointBlue);
            painter.setPen(QColor(r, g, b));
            painter.drawPoint(x, y);
        }
    }
}
//-----------------------------------------------------------------------------
