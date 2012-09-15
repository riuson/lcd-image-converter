#include "prepareoptions.h"
//-----------------------------------------------------------------------------
PrepareOptions::PrepareOptions(QObject *parent) :
    QObject(parent)
{
    this->mConvType = ConversionTypeMonochrome;
    this->mMonoType = MonochromeTypeDiffuseDither;
    this->mEdge = 128;
    this->mRotate = RotateNone;
    this->mFlipVertical = false;
    this->mFlipHorizontal = false;
    this->mInverse = false;
}
//-----------------------------------------------------------------------------
ConversionType PrepareOptions::convType() const
{
    if (this->mConvType <= ConversionTypeColor)
    {
        return this->mConvType;
    }
    return ConversionTypeColor;
}
//-----------------------------------------------------------------------------
MonochromeType PrepareOptions::monoType() const
{
    if (this->mMonoType <= MonochromeTypeThresholdDither)
    {
        return this->mMonoType;
    }
    return MonochromeTypeThresholdDither;
}
//-----------------------------------------------------------------------------
int PrepareOptions::edge() const
{
    if (this->mEdge <= 255)
    {
        return this->mEdge;
    }
    return 128;
}
//-----------------------------------------------------------------------------
Rotate PrepareOptions::rotate() const
{
    if (this->mRotate <= Rotate270)
    {
        return this->mRotate;
    }
    return Rotate270;
}
//-----------------------------------------------------------------------------
bool PrepareOptions::flipVertical() const
{
    return this->mFlipVertical;
}
//-----------------------------------------------------------------------------
bool PrepareOptions::flipHorizontal() const
{
    return this->mFlipHorizontal;
}
//-----------------------------------------------------------------------------
bool PrepareOptions::inverse() const
{
    return this->mInverse;
}
//-----------------------------------------------------------------------------
void PrepareOptions::setConvType(ConversionType value)
{
    if (value < ConversionTypeMonochrome || value > ConversionTypeColor)
        value = ConversionTypeColor;
    this->mConvType = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setMonoType(MonochromeType value)
{
    if (value < MonochromeTypeEdge || value > MonochromeTypeThresholdDither)
        value = MonochromeTypeDiffuseDither;
    this->mMonoType = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setEdge(int value)
{
    if (value < 0 || value > 255)
        value = 128;
    this->mEdge = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setRotate(Rotate value)
{
    if (value < RotateNone || value > Rotate270)
        value = RotateNone;
    this->mRotate = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setFlipVertical(bool value)
{
    this->mFlipVertical = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setFlipHorizontal(bool value)
{
    this->mFlipHorizontal = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setInverse(bool value)
{
    this->mInverse = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
const QString & PrepareOptions::convTypeName() const
{
    static const QString names[] =
    {
        "Monochrome",
        "Grayscale",
        "Color",
        "???"
    };
    switch (this->convType())
    {
    case ConversionTypeMonochrome:
        return names[0];
    case ConversionTypeGrayscale:
        return names[1];
    case ConversionTypeColor:
        return names[2];
    default:
        return names[3];
    }
}
//-----------------------------------------------------------------------------
const QString & PrepareOptions::monoTypeName() const
{
    static const QString names[] =
    {
        "Edge",
        "Diffuse Dither",
        "Ordered Dither",
        "Threshold Dither",
        "???"
    };
    switch (this->monoType())
    {
    case MonochromeTypeEdge:
        return names[0];
    case MonochromeTypeDiffuseDither:
        return names[1];
    case MonochromeTypeOrderedDither:
        return names[2];
    case MonochromeTypeThresholdDither:
        return names[3];
    default:
        return names[4];
    }
}
//-----------------------------------------------------------------------------
