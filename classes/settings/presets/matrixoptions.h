/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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

#ifndef MATRIXOPTIONS_H
#define MATRIXOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
template <class T> class QVector;
class QSettings;
class QDomElement;
//-----------------------------------------------------------------------------
class MatrixOptions : public QObject
{
    Q_OBJECT
public:
    explicit MatrixOptions(QObject *parent = 0);
    virtual ~MatrixOptions();

    quint32 maskUsed() const;
    quint32 maskAnd() const;
    quint32 maskOr() const;
    quint32 maskFill() const;

    void setMaskUsed(quint32 value);
    void setMaskAnd(quint32 value);
    void setMaskOr(quint32 value);
    void setMaskFill(quint32 value);

    int operationsCount() const;
    void operation(int index, quint32 *mask, int *shift, bool *left) const;
    void operationAdd(quint32 mask, int shift, bool left);
    void operationRemove(int index);
    void operationsRemoveAll();
    void operationReplace(int index, quint32 mask, int shift, bool left);

    bool load(QSettings *settings, int version);
    bool loadXmlElement(QDomElement *element);
    void save(QSettings *settings);
    void saveXmlElement(QDomElement *element);

private:
    static const QString GroupName;
    static const QString FieldMaskUsed;
    static const QString FieldMaskAnd;
    static const QString FieldMaskOr;
    static const QString FieldMaskFill;
    static const QString FieldOperations;
    static const QString FieldOperation;
    static const QString FieldMask;
    static const QString FieldShift;
    static const QString FieldLeft;

    quint32 mMaskUsed;
    quint32 mMaskAnd;
    quint32 mMaskOr;
    quint32 mMaskFill;
    QVector<quint32> *mOperations;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // MATRIXOPTIONS_H
