#include "convertergrayscale.h"

#include <QSettings>
#include <QImage>
#include <QPainter>
#include <QRgb>
//-----------------------------------------------------------------------------
ConverterGrayscale::ConverterGrayscale(QObject *parent) :
        QObject(parent)
{
    this->mBytesOrder = BigEndian;
    this->mDataLength = Data8;
    this->mMirrorBytes = false;
    this->mPack = true;
    this->mBitsPerPoint = 2;
}
//-----------------------------------------------------------------------------
ConverterGrayscale::~ConverterGrayscale()
{
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::loadSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("grayscale");

    bool ok;
    int a = sett.value("endian", QVariant(BigEndian)).toInt(&ok);
    if (ok)
        this->mBytesOrder = (BytesOrder)a;

    a = sett.value("dataLength", QVariant(Data8)).toInt(&ok);
    if (ok)
        this->mDataLength = (DataLength)a;

    this->mMirrorBytes = sett.value("mirror", QVariant(false)).toBool();
    this->mPack = sett.value("pack", QVariant(true)).toBool();

    a = sett.value("bitsPerPoint", QVariant(2)).toInt(&ok);
    if (ok)
        this->mBitsPerPoint = a;

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::saveSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("grayscale");

    sett.setValue("endian", QVariant(this->mBytesOrder));

    sett.setValue("dataLength", QVariant(this->mDataLength));

    sett.setValue("mirror", QVariant(this->mMirrorBytes));
    sett.setValue("pack", QVariant(this->mPack));

    sett.setValue("bitsPerPoint", QVariant(this->mBitsPerPoint));

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
QString ConverterGrayscale::name()
{
    return "grayscale";
}
//-----------------------------------------------------------------------------
QString ConverterGrayscale::displayName()
{
    return tr("Grayscale");
}
//-----------------------------------------------------------------------------
QImage ConverterGrayscale::preprocessImage(const QImage &source)
{
    QImage result(source);
    this->makeGrayscale(result);
    return result;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::processImage(const QImage &preprocessedImage, BitmapData *output)
{
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::options(BytesOrder *orderBytes,
                            DataLength *length,
                            bool *mirror,
                            bool *pack,
                            int *bitsPerPoint)
{
    *orderBytes = this->mBytesOrder;
    *length = this->mDataLength;
    *mirror = this->mMirrorBytes;
    *pack = this->mPack;
    *bitsPerPoint = this->mBitsPerPoint;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::setOptions(const BytesOrder &orderBytes,
                               const DataLength &length,
                               const bool mirror,
                               const bool pack,
                               const int bitsPerPoint)
{
    this->mBytesOrder = orderBytes;
    this->mDataLength = length;
    this->mMirrorBytes = mirror;
    this->mPack = pack;
    this->mBitsPerPoint = bitsPerPoint;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::makeGrayscale(QImage& image)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, false);
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            QRgb value = image.pixel(x, y);
            value = qGray(value);
            value = value & (0x0000ff00 >> this->mBitsPerPoint);
            painter.setPen(QColor(value, value, value));
            painter.drawPoint(x, y);
        }
    }
}
