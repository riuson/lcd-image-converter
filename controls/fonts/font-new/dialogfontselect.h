/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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
class QItemSelection;
class UnicodeBlocksModel;
class UnicodeBlocksFilterModel;
//-----------------------------------------------------------------------------
class DialogFontSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFontSelect(QWidget *parent = 0);
    ~DialogFontSelect();

    QString fontFamily();
    Q_DECL_DEPRECATED QString fontStyle();
    int fontSize();
    bool monospaced();
    bool antialiasing();
    bool bold();
    bool italic();
    QString characters();

    void setFontFamily(const QString &value);
    Q_DECL_DEPRECATED void setFontStyle(const QString &value);
    void setFontSize(int value);
    void setMonospaced(bool value);
    void setAntialising(bool value);
    void setBold(bool value);
    void setItalic(bool value);
    void setCharacters(const QString &value);
private:
    Ui::DialogFontSelect *ui;
    CharactersModel *mModel;
    UnicodeBlocksModel *mBlocksModel;
    UnicodeBlocksFilterModel *mBlocksFilterModel;
    bool mSortOrderUp;

    QString mFontFamily;
    Q_DECL_DEPRECATED QString mFontStyle;
    int mSize;
    bool mMonospaced;
    bool mAntialiasing;
    bool mBold;
    bool mItalic;
    QString mCharacters;

    void updateFont();
    Q_DECL_DEPRECATED void updateStyles();
    void updateSizes();

    void applyFont();
    QString appendCharacters(const QString &original, const QString &value);
    QString editorText();
    void setEditorText(const QString &value);

private slots:
    void on_fontComboBox_currentFontChanged(const QFont &font);
    void on_comboBoxSize_currentIndexChanged(const QString &text);
    void on_comboBoxSize_editTextChanged(const QString & text);
    void on_comboBoxStyle_currentIndexChanged(const QString &text);
    void on_radioButtonProportional_toggled(bool value);
    void on_checkBoxAntialiasing_toggled(bool value);
    void on_checkBoxBold_toggled(bool value);
    void on_checkBoxItalic_toggled(bool value);
    void on_lineEdit_textChanged();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButtonAppendSelected_clicked();
    void on_pushButtonAppendRange_clicked();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void rangeChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void on_lineEditUnicodeBlocksFilter_textChanged(const QString & text);
    void on_pushButtonSort_clicked();
};

#endif // DIALOGFONTSELECT_H
//-----------------------------------------------------------------------------
