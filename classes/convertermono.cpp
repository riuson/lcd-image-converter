#include "convertermono.h"

#include <QSettings>
#include <QImage>
#include <QPainter>

#include "bitmapdata.h"
//-----------------------------------------------------------------------------
ConverterMono::ConverterMono(QObject *parent) :
        QObject(parent)
{
    this->mBytesOrder = BigEndian;
    this->mDataLength = Data8;
    this->mMirrorBytes = false;
    this->mBlackWhiteLevel = 128;
    this->mDithType = Edge;
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

    a = sett.value("dithType", QVariant(Edge)).toInt(&ok);
    if (ok)
        this->mDithType = (ConvMonoType)a;

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

    sett.setValue("dithType", QVariant(this->mDithType));

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
QImage ConverterMono::preprocessImage(const QImage &source)
{
    QImage result(source);
    switch (this->mDithType)
    {
    case Edge:
        this->makeMonochrome(result);
        break;
    case DiffuseDither:
        result = result.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
        break;
    case OrderedDither:
        result = result.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
        break;
    case ThresholdDither:
        result = result.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
        break;
    }

    return result;
}
//-----------------------------------------------------------------------------
void ConverterMono::processImage(const QImage &image, BitmapData *output)
{
    output->clear();
    output->setBlockSize(this->mDataLength);
    output->setWidth(image.width());
    output->setHeight(image.height());

    quint32 data = 0;
    int bitsCounter = 0;
    for (int y = 0; y < image.height(); y++)
    {
        bitsCounter = 0;
        for (int x = 0; x < image.width(); x++)
        {
            QRgb point = image.pixel(x, y);
            // if point is black
            if (qRed(point) == 0)
                data |= (0x01 << (this->mDataLength - bitsCounter - 1));
            bitsCounter++;
            if (bitsCounter >= this->mDataLength)
            {
                output->addBlock(data);
                data = 0;
                bitsCounter = 0;
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
void ConverterMono::options(BytesOrder *orderBytes,
                            DataLength *length,
                            bool *mirror,
                            int *level,
                            ConvMonoType *dithType)
{
    *orderBytes = this->mBytesOrder;
    *length = this->mDataLength;
    *mirror = this->mMirrorBytes;
    *level = this->mBlackWhiteLevel;
    *dithType = this->mDithType;
}
//-----------------------------------------------------------------------------
void ConverterMono::setOptions(const BytesOrder &orderBytes,
                               const DataLength &length,
                               const bool mirror,
                               const int level,
                               const ConvMonoType dithType)
{
    this->mBytesOrder = orderBytes;
    this->mDataLength = length;
    this->mMirrorBytes = mirror;
    this->mBlackWhiteLevel = level;
    this->mDithType = dithType;
}
//-----------------------------------------------------------------------------
void ConverterMono::makeMonochrome(QImage &image)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, false);
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            QRgb value = image.pixel(x, y);
            if (qGray(value) < this->mBlackWhiteLevel)
                painter.setPen(QColor(0, 0, 0));
            else
                painter.setPen(QColor(255, 255, 255));
            painter.drawPoint(x, y);
        }
    }
}//-----------------------------------------------------------------------------
