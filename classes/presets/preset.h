#ifndef PRESET_H
#define PRESET_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class PrepareOptions;
class MatrixOptions;
class ImageOptions;
class TemplateOptions;
class QString;
class QStringList;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class Preset : public QObject
{
    Q_OBJECT
public:
    explicit Preset(QObject *parent = 0);
    virtual ~Preset();

    PrepareOptions *prepare();
    MatrixOptions *matrix();
    ImageOptions *image();
    TemplateOptions *templates();

    static QStringList presetsList();

    bool load(const QString &name);
    bool save(const QString &name) const;

    void initMono(MonochromeType type, int edge = 0x80);
    void initGrayscale(int bits);
    void initColor(int redBits, int greenBits, int blueBits);

private:
    PrepareOptions *mPrepare;
    MatrixOptions *mMatrix;
    ImageOptions *mImage;
    TemplateOptions *mTemplates;
    bool mBlockChangesSignal;

    bool load1(const QString &name);
    bool save1(const QString &name) const;

signals:
    void changed();

public slots:

private slots:
    void partsChanged();

};
//-----------------------------------------------------------------------------
#endif // PRESET_H
