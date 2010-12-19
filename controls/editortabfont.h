#ifndef EDITORTABFONT_H
#define EDITORTABFONT_H
//-----------------------------------------------------------------------------
#include <QWidget>

#include "idocument.h"
//-----------------------------------------------------------------------------
namespace Ui {
    class EditorTabFont;
}
//-----------------------------------------------------------------------------
class FontContainer;
class WidgetBitmapEditor;
class QSplitter;
//-----------------------------------------------------------------------------
class EditorTabFont : public QWidget, public IDocument
{
    Q_OBJECT
    Q_INTERFACES(IDocument)

public:
    explicit EditorTabFont(QWidget *parent = 0);
    ~EditorTabFont();

    bool load(const QString &fileName);
    bool save(const QString &fileName);
    bool changed();
    QString fileName();
    QString documentName();
    void setDocumentName(const QString &value);

    WidgetBitmapEditor *editor();

    void assignFontCharacters(const QString &chars,
                              const QString &fontFamily,
                              const QString &style,
                              const int size);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabFont *ui;
    WidgetBitmapEditor *mEditor;
    FontContainer *mContainer;
    QSplitter *mSplitter;

    QString mFileName;
    QString mDocumentName;
    bool mDataChanged;
    QFont mFont;

private slots:
    void on_editor_dataChanged();
signals:
    void dataChanged();
};
//-----------------------------------------------------------------------------
#endif // EDITORTABFONT_H
