#ifndef DIALOGFONTRANGE_H
#define DIALOGFONTRANGE_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
class DialogFontRange;
}
//-----------------------------------------------------------------------------
class DialogFontRange : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFontRange(QWidget *parent = 0);
    ~DialogFontRange();

    const QString &resultString() const;

private:
    Ui::DialogFontRange *ui;
    QString mResultString;

private slots:
    void updatePreview();
    void updatePreview(const QString &encoding, int from, int to, bool bigEndian);
    void on_plainTextEditPreview_textChanged();
};
//-----------------------------------------------------------------------------
#endif // DIALOGFONTRANGE_H
