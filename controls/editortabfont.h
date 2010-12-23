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
    IDataContainer *dataContainer();

    WidgetBitmapEditor *editor();

    void setFontCharacters(const QString &chars,
                           const QString &fontFamily,
                           const QString &style,
                           const int size,
                           const bool monospaced,
                           const bool antialiasing);
    void fontCharacters(QString *chars,
                        QString *fontFamily,
                        QString *style,
                        int *size,
                        bool *monospaced,
                        bool *antialiasing);

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

    //QString mCharacters;
    QString mStyle;
    bool mMonospaced;
    bool mAntialiasing;

    QImage drawCharacter(const QChar value,
                         const QFont &font,
                         const QColor &foreground,
                         const QColor &background,
                         const int width,
                         const int height,
                         const bool antialiasing);

private slots:
    void mon_editor_dataChanged();
    void on_listWidgetCharacters_currentTextChanged(const QString &value);
signals:
    void dataChanged();
};
//-----------------------------------------------------------------------------
#endif // EDITORTABFONT_H
