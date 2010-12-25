#ifndef DIALOGFONTSELECT_H
#define DIALOGFONTSELECT_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogFontSelect;
}
class CharactersModel;
class QModelIndex;
//-----------------------------------------------------------------------------
class DialogFontSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFontSelect(QWidget *parent = 0);
    ~DialogFontSelect();

    QString fontFamily();
    QString fontStyle();
    int fontSize();
    bool monospaced();
    bool antialiasing();
    QString characters();

    void setFontFamily(const QString &value);
    void setFontStyle(const QString &value);
    void setFontSize(int value);
    void setMonospaced(bool value);
    void setAntialising(bool value);
    void setCharacters(const QString &value);
private:
    Ui::DialogFontSelect *ui;
    CharactersModel *mModel;

    QString mFontFamily;
    QString mFontStyle;
    int mSize;
    bool mMonospaced;
    bool mAntialiasing;
    QString mCharacters;

    void updateFont();
    void updateStyles();
    void updateSizes();

    void applyFont();
private slots:
    void on_fontComboBox_currentFontChanged(const QFont &font);
    void on_comboBoxSize_currentIndexChanged(int index);
    void on_comboBoxStyle_currentIndexChanged(const QString &text);
    void on_radioButtonProportional_toggled(bool value);
    void on_checkBoxAntialiasing_toggled(bool value);
    void on_lineEdit_textChanged(const QString &value);
    void on_tableView_doubleClicked(const QModelIndex &index);
};

#endif // DIALOGFONTSELECT_H
//-----------------------------------------------------------------------------
