#include "convertergrayscale.h"

#include <QSettings>
#include <QImage>
#include <QPainter>
#include <QRgb>
#include <QQueue>

#include "bitmapdata.h"
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
void ConverterGrayscale::processImage(const QImage &image, BitmapData *output)
{
    output->clear();
    output->setBlockSize(this->mDataLength);
    output->setWidth(image.width());
    output->setHeight(image.height());

    QQueue<bool> temp;// queue for bits of line

    for (int y = 0; y < image.height(); y++)
    {
        temp.clear();
        // collect all bits of line
        for (int x = 0; x < image.width(); x++)
        {
            QRgb point = image.pixel(x, y);// r = g = b
            quint8 r = qRed(point);
            for (int i = 0; i < this->mBitsPerPoint; i++)
            {
                if (r & (0x80 >> i))
                    temp.enqueue(true);
                else
                    temp.enqueue(false);
            }
        }
        // put bits to data blocks
        quint32 data = 0;
        int bitsCounter = 0;

        while (!temp.isEmpty())
        {
            bool bit = temp.dequeue();
            if (bit)
                data |= (0x01 << (this->mDataLength - bitsCounter - 1));
            bitsCounter++;
            // if need packing
            if (this->mPack)
            {
                if (bitsCounter >= this->mDataLength)
                {
                    output->addBlock(data);
                    data = 0;
                    bitsCounter = 0;
                }
            }
            else
            {
                // if data block full
                if (bitsCounter >= this->mDataLength - (this->mDataLength % this->mBitsPerPoint))
                {
                    bitsCounter = 0;
                    output->addBlock(data);
                    data = 0;
                }
            }
        }
        if (bitsCounter != 0)
        {
            output->addBlock(data);
            data = 0;
            bitsCounter = 0;
        }
    }

    if (this->mBytesOrder == LittleEndian)
        output->swapBytes();
    if (this->mMirrorBytes)
        output->mirrorBytes();
}
//-----------------------------------------------------------------------------
IConverter::BytesOrder ConverterGrayscale::order()
{
    return this->mBytesOrder;
}
//-----------------------------------------------------------------------------
IConverter::DataLength ConverterGrayscale::length()
{
    return this->mDataLength;
}
//-----------------------------------------------------------------------------
bool ConverterGrayscale::mirror()
{
    return this->mMirrorBytes;
}
//-----------------------------------------------------------------------------
bool ConverterGrayscale::pack()
{
    return this->mPack;
}
//-----------------------------------------------------------------------------
int ConverterGrayscale::depth()
{
    return this->mBitsPerPoint;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::setOrder(BytesOrder value)
{
    this->mBytesOrder = value;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::setLength(DataLength value)
{
    this->mDataLength = value;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::setMirror(bool value)
{
    this->mMirrorBytes = value;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::setPack(bool value)
{
    this->mPack = value;
}
//-----------------------------------------------------------------------------
void ConverterGrayscale::setDepth(int value)
{
    this->mBitsPerPoint = value;
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
