#ifndef EDITORTABIMAGE_H
#define EDITORTABIMAGE_H
//-----------------------------------------------------------------------------
#include <QWidget>

#include "idocument.h"
//-----------------------------------------------------------------------------
namespace Ui {
    class EditorTabImage;
}
//-----------------------------------------------------------------------------
class WidgetBitmapEditor;
class BitmapContainer;
//-----------------------------------------------------------------------------
class EditorTabImage : public QWidget, public IDocument
{
    Q_OBJECT
    Q_INTERFACES(IDocument)

public:
    explicit EditorTabImage(QWidget *parent = 0);
    ~EditorTabImage();

    bool load(const QString &fileName);
    bool save(const QString &fileName);
    bool changed();
    QString fileName();
    QString documentName();
    void setDocumentName(const QString &value);

    WidgetBitmapEditor *editor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabImage *ui;
    WidgetBitmapEditor *mEditor;
    BitmapContainer *mContainer;

    QString mFileName;
    QString mDocumentName;
    bool mDataChanged;
private slots:
    void mon_editor_dataChanged();
signals:
    void dataChanged();
};
//-----------------------------------------------------------------------------
#endif // EDITORTABIMAGE_H
