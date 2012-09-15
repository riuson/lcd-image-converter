#include "preset.h"
//-----------------------------------------------------------------------------
#include <QStringList>
#include <QSettings>
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
#include "fontoptions.h"
#include "templateoptions.h"
//-----------------------------------------------------------------------------
Preset::Preset(QObject *parent) :
    QObject(parent)
{
    this->mBlockChangesSignal = false;

    this->mPrepare   = new PrepareOptions(this);
    this->mMatrix    = new MatrixOptions(this);
    this->mImage     = new ImageOptions(this);
    this->mFont     = new FontOptions(this);
    this->mTemplates = new TemplateOptions(this);

    this->connect(this->mPrepare,   SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mMatrix,    SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mImage,     SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mFont,      SIGNAL(changed()), SLOT(partsChanged()));
    this->connect(this->mTemplates, SIGNAL(changed()), SLOT(partsChanged()));
}
//-----------------------------------------------------------------------------
Preset::~Preset()
{
    delete this->mTemplates;
    delete this->mFont;
    delete this->mImage;
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
bool Preset::load(const QString &name)
{
    bool result = false;

    this->mBlockChangesSignal = true;

    QSettings sett;
    sett.beginGroup("presets");

    // get version of settings
    int version;
    bool ok;
    QVariant varVersion = sett.value("version", QVariant((int)1));
    version = varVersion.toInt(&ok);
    if (ok)
    {
        switch (version)
        {
        case 1:
            result = this->load1(name);
            break;
        case 2:
            break;
        }
    }

    sett.endGroup();

    this->mBlockChangesSignal = false;

    emit this->changed();

    return result;
}
//-----------------------------------------------------------------------------
bool Preset::save(const QString &name) const
{
    return this->save1(name);
}
//-----------------------------------------------------------------------------
void Preset::initMono(MonochromeType type, int edge)
{
    this->mMatrix->operationsRemoveAll();

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
bool Preset::load1(const QString &name)
{
    if (name.isEmpty())
        return false;

    QSettings sett;
    sett.beginGroup("presets");

    if (sett.childGroups().contains(name))
    {
        sett.beginGroup(name);

        bool ok = false;
        quint32 bytesOrder = 0, convType = 0, monoType = 0, edge = 0, blockSize = 0;
        quint32 rotate = 0, flipVertical = 0, flipHorizontal = 0, inverse = 0;
        quint32 maskUsed = 0, maskAnd = 0, maskOr = 0, maskFill = 0;
        quint32 fontUseBom = 0;

        bytesOrder = sett.value("bytesOrder", int(0)).toInt(&ok);

        if (ok)
            convType = sett.value("convType", int(0)).toInt(&ok);

        if (ok)
            monoType = sett.value("monoType", int(0)).toInt(&ok);

        if (ok)
            edge = sett.value("edge", int(0)).toInt(&ok);

        if (ok)
            blockSize = sett.value("blockSize", int(0)).toInt(&ok);

        if (ok)
            rotate = sett.value("rotate", int(0)).toInt(&ok);

        if (ok)
            flipVertical = sett.value("flipVertical", int(0)).toInt(&ok);

        if (ok)
            flipHorizontal = sett.value("flipHorizontal", int(0)).toInt(&ok);

        if (ok)
            inverse = sett.value("inverse", int(0)).toInt(&ok);

        QString strMaskUsed = sett.value("maskUsed", QString("ffffffff")).toString();
        QString strMaskAnd = sett.value("maskAnd", QString("ffffffff")).toString();
        QString strMaskOr = sett.value("maskOr", QString("00000000")).toString();
        QString strMaskFill = sett.value("maskFill", QString("ffffffff")).toString();

        if (ok)
            maskUsed = strMaskUsed.toUInt(&ok, 16);

        if (ok)
            maskAnd = strMaskAnd.toUInt(&ok, 16);

        if (ok)
            maskOr = strMaskOr.toUInt(&ok, 16);

        if (ok)
            maskFill = strMaskFill.toUInt(&ok, 16);

        QString strTemplateImage = sett.value("templateImage", QString(":/templates/image_convert")).toString();
        QString strTemplateFont = sett.value("templateFont", QString(":/templates/font_convert")).toString();

        QString strFontCodec = sett.value("fontCodec", QString("UTF-8")).toString();

        if (ok)
            fontUseBom = sett.value("fontUseBom", int(0)).toInt(&ok);

        if (ok)
        {
            this->mPrepare->setConvType((ConversionType)convType);
            this->mPrepare->setMonoType((MonochromeType)monoType);
            this->mPrepare->setEdge((int)edge);
            this->mPrepare->setRotate((Rotate)rotate);
            this->mPrepare->setFlipVertical((bool)flipVertical);
            this->mPrepare->setFlipHorizontal((bool)flipHorizontal);
            this->mPrepare->setInverse((bool)inverse);

            this->mMatrix->setMaskUsed(maskUsed);
            this->mMatrix->setMaskAnd(maskAnd);
            this->mMatrix->setMaskOr(maskOr);
            this->mMatrix->setMaskFill(maskFill);

            this->mImage->setBytesOrder((BytesOrder)bytesOrder);
            this->mImage->setBlockSize((DataBlockSize)blockSize);

            int operations = sett.beginReadArray("matrix");

            this->mMatrix->operationsRemoveAll();

            for (int i = 0; i < operations; i++)
            {
                sett.setArrayIndex(i);

                QString strMask = sett.value("mask", QString("00000000")).toString();
                quint32 mask, shift, left;
                if (ok)
                    mask = strMask.toUInt(&ok, 16);

                if (ok)
                    shift = sett.value("shift", uint(0)).toUInt(&ok);

                if (ok)
                    left = sett.value("left", uint(0)).toUInt(&ok);

                if (ok)
                {
                    this->mMatrix->operationAdd(mask, shift, left != 0);
                }
            }
            sett.endArray();

            this->mTemplates->setImage(strTemplateImage);
            this->mTemplates->setFont(strTemplateFont);

            this->mFont->setEncoding(strFontCodec);
            this->mFont->setBom((bool)fontUseBom);
        }

        sett.endGroup();
    }
    sett.endGroup();

    emit this->changed();

    return true;
}
//-----------------------------------------------------------------------------
bool Preset::save1(const QString &name) const
{
    QSettings sett;
    sett.beginGroup("presets");

    sett.beginGroup(name);
    sett.remove("");

    sett.setValue("convType",       QString("%1").arg((int)this->mPrepare->convType()));
    sett.setValue("monoType",       QString("%1").arg((int)this->mPrepare->monoType()));
    sett.setValue("edge",           QString("%1").arg((int)this->mPrepare->edge()));
    sett.setValue("rotate",         QString("%1").arg((int)this->mPrepare->rotate()));
    sett.setValue("flipVertical",   QString("%1").arg((int)this->mPrepare->flipVertical()));
    sett.setValue("flipHorizontal", QString("%1").arg((int)this->mPrepare->flipHorizontal()));
    sett.setValue("inverse",        QString("%1").arg((int)this->mPrepare->inverse()));

    sett.setValue("maskUsed", QString("%1").arg(this->mMatrix->maskUsed(), 8, 16, QChar('0')));
    sett.setValue("maskAnd",  QString("%1").arg(this->mMatrix->maskAnd(),  8, 16, QChar('0')));
    sett.setValue("maskOr",   QString("%1").arg(this->mMatrix->maskOr(),   8, 16, QChar('0')));
    sett.setValue("maskFill", QString("%1").arg(this->mMatrix->maskFill(), 8, 16, QChar('0')));

    sett.setValue("bytesOrder",     QString("%1").arg((int)this->mImage->bytesOrder()));
    sett.setValue("blockSize",      QString("%1").arg((int)this->mImage->blockSize()));

    sett.beginWriteArray("matrix");

    for (int i = 0; i < this->mMatrix->operationsCount(); i++)
    {
        quint32 mask;
        int shift;
        bool left;
        this->mMatrix->operation(i, &mask, &shift, &left);

        sett.setArrayIndex(i);
        sett.setValue("mask",  QString("%1").arg(mask, 8, 16, QChar('0')));
        sett.setValue("shift", QString("%1").arg(shift));
        sett.setValue("left",  QString("%1").arg((int)left));
    }
    sett.endArray();

    sett.setValue("imageTemplate", this->mTemplates->image());
    sett.setValue("fontTemplate", this->mTemplates->font());

    sett.setValue("fontUseBom", QString("%1").arg((int)this->mFont->bom()));
    sett.setValue("fontCodec",  this->mFont->encoding());

    sett.endGroup();
    sett.endGroup();

    return true;
}
//-----------------------------------------------------------------------------
void Preset::partsChanged()
{
    if (!this->mBlockChangesSignal)
        emit this->changed();
}
//-----------------------------------------------------------------------------
