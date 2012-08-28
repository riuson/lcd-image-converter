/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#ifndef CONVERSIONMATRIXOPTIONS_H
#define CONVERSIONMATRIXOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class QStringList;
class QImage;
template <class T> class QVector;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
/*
 *  Matrix structure:
 *
 * [0] ConversionOptions:
 *     31    - not used more
 *     30    - byte order, 0 = little-endian, 1 = big-endian
 *     29:28 - conversion type
 *     27:24 - monochrome type
 *     23:16 - edge value for MonochromeTypeEdge
 *     15:12 - data block size
 *     11:10 - preprocess roration
 *     9     - preprocess flip vertical
 *     8     - preprocess flip horizontal
 *     7     - preprocess inverse
 *
 * [1] Mask of used bits before packing
 *
 * [2] Mask AND of bits before packing
 * [3] Mask OR of bits before packing
 *
 * [4] Mask of data bit
 * [5] Shift of data bit:
 *     31    - 0 = to right, 1 = to left
 *     4:0   - count
 *
 * ... Repeat 4 and 5 some times
 */

class ConversionMatrixOptions : public QObject
{
    Q_OBJECT
public:
    explicit ConversionMatrixOptions(QVector<quint32> *matrix, QObject *parent = 0);
    virtual ~ConversionMatrixOptions();

    BytesOrder bytesOrder() const;
    ConversionType convType() const;
    MonochromeType monoType() const;
    int edge() const;
    DataBlockSize blockSize() const;
    quint32 maskUsed() const;
    quint32 maskAnd() const;
    quint32 maskOr() const;
    quint32 maskFill() const;
    Rotate rotate() const;
    bool flipVertical() const;
    bool flipHorizontal() const;
    bool inverse() const;

    void setBytesOrder(BytesOrder value);
    void setConvType(ConversionType value);
    void setMonoType(MonochromeType value);
    void setEdge(int value);
    void setBlockSize(DataBlockSize value);
    void setMaskUsed(quint32 value);
    void setMaskAnd(quint32 value);
    void setMaskOr(quint32 value);
    void setMaskFill(quint32 value);
    void setRotate(Rotate value);
    void setFlipVertical(bool value);
    void setFlipHorizontal(bool value);
    void setInverse(bool value);

    const QString & convTypeName() const;
    const QString & monoTypeName() const;

private:

    enum
    {
        IndexBytesOrder               = 0,
        IndexConversionType           = 1,
        IndexMonochromeType           = 2,
        IndexEdge                     = 3,
        IndexDataBlockSize            = 4,
        IndexPreprocessRotation       = 5,
        IndexPreprocessFlipVertical   = 6,
        IndexPreprocessFlipHorizontal = 7,
        IndexPreprocessInverse        = 8,
        IndexMaskUsed                 = 9,
        IndexMaskAnd                  = 10,
        IndexMaskOr                    = 11,
        IndexMaskFill                 = 12,
        IndexesCount                  = 13
    };

    QVector<quint32> *mMatrixData;

    quint32 get(int index) const;
    void    set(int index, quint32 value);

signals:
    void changed();
};
//-----------------------------------------------------------------------------
#endif // CONVERSIONMATRIXOPTIONS_H
