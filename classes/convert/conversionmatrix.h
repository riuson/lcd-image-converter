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

#ifndef CONVERSIONMATRIX_H
#define CONVERSIONMATRIX_H
//-----------------------------------------------------------------------------
#include <QObject>
#include "conversionmatrixoptions.h"
//-----------------------------------------------------------------------------
template <class T> class QVector;
//-----------------------------------------------------------------------------
class ConversionMatrix : public QObject
{
    Q_OBJECT
public:
    explicit ConversionMatrix(QObject *parent = 0);
    virtual ~ConversionMatrix();

    ConversionMatrixOptions *options() const;

    int operationsCount() const;
    void operation(int index, quint32 *mask, int *shift, bool *left) const;
    void operationAdd(quint32 mask, int shift, bool left);
    void operationRemove(int index);
    void operationsRemoveAll();
    void operationReplace(int index, quint32 mask, int shift, bool left);

    void init(quint32 flags, quint32 maskUsed, quint32 maskAnd, quint32 maskOr, quint32 maskFill);
    void initMono(MonochromeType type, int edge = 0x80);
    void initGrayscale(int bits);
    void initColor(int redBits, int greenBits, int blueBits);

    bool load(const QString &name);
    bool save(const QString &name) const;

private:
    QVector<quint32> *mData;
    ConversionMatrixOptions *mOptions;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // CONVERSIONMATRIX_H
