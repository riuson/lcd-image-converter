#ifndef EDITOR_H
#define EDITOR_H
//-----------------------------------------------------------------------------
#include <QObject>
#include "iimageeditor.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
class WidgetEditor;

class Editor : public QObject, public IImageEditor
{
    Q_OBJECT
    Q_INTERFACES(ImageEditor::IImageEditor)

public:
    explicit Editor(QObject *parent = 0);
    ~Editor();

    QWidget *widget() const;

    const QImage *image() const;
    void setImage(const QImage *_value);

    const QColor *foreColor() const;
    const QColor *backColor() const;

signals:
    void imageChanged(const QImage *_image);
    void scaleChanged(int _scale);
    void mouseMoved(const QPoint *_point);

signals:

public slots:

private:
    WidgetEditor *mWidget;
};
}
//-----------------------------------------------------------------------------
#endif // EDITOR_H
