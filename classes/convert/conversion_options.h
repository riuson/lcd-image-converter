#ifndef CONVERSION_OPTIONS_H
#define CONVERSION_OPTIONS_H
//------------------------------------------------------------------------------
#include <QtGlobal>
//------------------------------------------------------------------------------
namespace ConversionOptions {

//------------------------------------------------------------------------------
enum ConversionType
{
    ConversionTypeMonochrome = 0,
    ConversionTypeGrayscale  = 1,
    ConversionTypeColor      = 2
};
//-----------------------------------------------------------------------------
enum MonochromeType
{
    MonochromeTypeEdge = 0,
    MonochromeTypeDiffuseDither = 1,
    MonochromeTypeOrderedDither = 2,
    MonochromeTypeThresholdDither = 3
};
//-----------------------------------------------------------------------------
enum ColorsOrder
{
    ColorsOrderRGB = 0,
    ColorsOrderRBG = 1,
    ColorsOrderGRB = 2,
    ColorsOrderGBR = 3,
    ColorsOrderBRG = 4,
    ColorsOrderBGR = 5
};
//-----------------------------------------------------------------------------
enum BytesOrder
{
    BytesOrderLittleEndian,
    BytesOrderBigEndian
};
//-----------------------------------------------------------------------------
enum DataBlockSize
{
    Data8  = 0,
    Data16 = 1,
    Data24 = 2,
    Data32 = 3
};
//------------------------------------------------------------------------------
enum Transformation
{
    TransformNone = 0,
    TransformRotate90 = 1,
    TransformRotate180 = 2,
    TransformRotate270 = 3,
    TransformRotatations = 3,
    TransformFlipHorizontal = 4,
    TransformFlipVertical = 8,
    TransformInverse = 16
};
Q_DECLARE_FLAGS(Transformations, Transformation)
//------------------------------------------------------------------------------

}
//------------------------------------------------------------------------------
#endif // CONVERSION_OPTIONS_H
