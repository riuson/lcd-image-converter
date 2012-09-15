#ifndef PREPAREOPTIONS_H
#define PREPAREOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class PrepareOptions : public QObject
{
    Q_OBJECT
public:
    explicit PrepareOptions(QObject *parent = 0);

    ConversionType convType() const;
    MonochromeType monoType() const;
    int edge() const;
    Rotate rotate() const;
    bool flipVertical() const;
    bool flipHorizontal() const;
    bool inverse() const;

    void setConvType(ConversionType value);
    void setMonoType(MonochromeType value);
    void setEdge(int value);
    void setRotate(Rotate value);
    void setFlipVertical(bool value);
    void setFlipHorizontal(bool value);
    void setInverse(bool value);

    const QString & convTypeName() const;
    const QString & monoTypeName() const;

private:
    ConversionType mConvType;
    MonochromeType mMonoType;
    int mEdge;
    Rotate mRotate;
    bool mFlipVertical;
    bool mFlipHorizontal;
    bool mInverse;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // PREPAREOPTIONS_H
