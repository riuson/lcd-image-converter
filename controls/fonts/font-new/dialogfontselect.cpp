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

#include "dialogfontselect.h"
#include "ui_dialogfontselect.h"

#include <QTableWidgetSelectionRange>
#include <QColorDialog>
#include "charactersmodel.h"
#include "unicodeblocksmodel.h"
#include "unicodeblocksfiltermodel.h"
#include "dialogfontrange.h"
#include "fonthelper.h"
#include "fonteditoroptions.h"
#include "tfontparameters.h"
//-----------------------------------------------------------------------------
DialogFontSelect::DialogFontSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontSelect)
{
    ui->setupUi(this);

    this->mModel = new CharactersModel(this);
    this->mModel->setCodesRange(0x0000, 0x00ff);
    this->ui->tableView->setModel(this->mModel);

    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    this->ui->radioButtonMonospaced->setChecked(false);
    this->ui->radioButtonProportional->setChecked(false);
    this->ui->checkBoxAntialiasing->setChecked(false);

    this->mSize = 14;
    this->mMonospaced = false;
    this->mAntialiasing = false;
    this->mAlphaChannel = false;

    QString defChars;
    for (int i = 0x20; i < 0x7f; i++)
    {
        defChars += QString(QChar(i));
    }
    this->setEditorText(defChars);

    this->mFontFamily = this->ui->fontComboBox->currentFont().family();
    this->updateStyles();

    this->mBlocksModel = new UnicodeBlocksModel(this);

    this->mBlocksFilterModel = new UnicodeBlocksFilterModel(this);
    this->mBlocksFilterModel->setSourceModel(this->mBlocksModel);

    this->ui->listViewBlocks->setModel(this->mBlocksFilterModel);
    this->mSortOrderUp = false;

    selectionModel = this->ui->listViewBlocks->selectionModel();
    this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(rangeChanged(QItemSelection,QItemSelection)));

    QPixmap pixmapForeColor = QPixmap(24, 24);
    pixmapForeColor.fill(FontEditorOptions::foreColor());
    this->ui->toolButtonForeColor->setIcon(QIcon(pixmapForeColor));

    QPixmap pixmapBackColor = QPixmap(24, 24);
    pixmapBackColor.fill(FontEditorOptions::backColor());
    this->ui->toolButtonBackColor->setIcon(QIcon(pixmapBackColor));
}
//-----------------------------------------------------------------------------
DialogFontSelect::~DialogFontSelect()
{
    delete ui;
}
//-----------------------------------------------------------------------------
QString DialogFontSelect::characters()
{
    return this->mCharacters;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::getFontParameters(tFontParameters *parameters)
{
    parameters->family = this->mFontFamily;
    parameters->style = this->mFontStyle;
    parameters->size = this->mSize;
    parameters->monospaced = this->mMonospaced;
    parameters->antiAliasing = this->mAntialiasing;
    parameters->alphaChannel = this->mAlphaChannel;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setCharacters(const QString &value)
{
    this->mCharacters = value;
    this->setEditorText(value);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setFontParameters(const tFontParameters &parameters)
{
    // Family
    {
        QFontDatabase fonts;
        QString style = this->mFontStyle;
        QFont f = fonts.font(parameters.family, style, this->mSize);
        f.setPixelSize(this->mSize);
        //this->mFontFamily = value;
        this->ui->fontComboBox->setCurrentFont(f);

        this->updateStyles();
        this->updateSizes();

        this->applyFont();
    }
    // Style
    {
        this->mFontStyle = parameters.style;
        int index = this->ui->comboBoxStyle->findText(parameters.style);
        if (index >= 0)
            this->ui->comboBoxStyle->setCurrentIndex(index);
        else
            this->ui->comboBoxStyle->setCurrentIndex(0);

        this->updateSizes();

        this->applyFont();
    }
    // Size
    {
        this->mSize = parameters.size;

        int index = this->ui->comboBoxSize->findText(QString("%1").arg(parameters.size));
        if (index >= 0)
            this->ui->comboBoxSize->setCurrentIndex(index);
        else
        {
            QString str = QString("%1").arg(parameters.size);
            this->ui->comboBoxSize->setEditText(str);
        }

        this->applyFont();
    }

    this->ui->radioButtonMonospaced->setChecked(parameters.monospaced);
    this->ui->checkBoxAntialiasing->setChecked(parameters.antiAliasing);
    this->ui->checkBoxAlphaChannel->setChecked(parameters.alphaChannel);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::updateFont()
{
    //QFontDatabase fonts;
    //QFont font = fonts.font(this->mFontFamily, this->mFontStyle, this->mSize);
    //font.setPixelSize(this->mSize);
    //this->ui->fontComboBox->setCurrentFont(font);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::updateStyles()
{
    QString style = this->mFontStyle;
    this->ui->comboBoxStyle->clear();

    QFontDatabase fonts;
    QStringList stylesList = fonts.styles(this->mFontFamily);
    this->ui->comboBoxStyle->addItems(stylesList);
    int index = stylesList.indexOf(style);
    if (index >= 0)
        this->ui->comboBoxStyle->setCurrentIndex(stylesList.indexOf(style));
    else
        this->ui->comboBoxStyle->setCurrentIndex(0);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::updateSizes()
{
    int a = this->mSize;
    //bool ok;

    QFontDatabase fonts;

    QList<int> sizes;
    if (fonts.isSmoothlyScalable(this->mFontFamily, this->mFontStyle))
        sizes = fonts.smoothSizes(this->mFontFamily, this->mFontStyle);
    else
        sizes = fonts.standardSizes();
    this->ui->comboBoxSize->clear();
    QListIterator<int> it(sizes);
    it.toFront();
    int b = 0, c = 0;
    while (it.hasNext())
    {
        int size = it.next();
        this->ui->comboBoxSize->addItem(QString("%1").arg(size), QVariant(size));
        if (a == 14 && c == 0)
            c = b;
        if (size == a)
            c = b;
        b++;
    }
    this->ui->comboBoxSize->setCurrentIndex(c);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::applyFont()
{
    QFontDatabase fonts;
    QFont font = fonts.font(this->mFontFamily, this->mFontStyle, this->mSize);
    font.setPixelSize(this->mSize);

    this->ui->tableView->setFont(font);

    QFontMetrics metrics(font);

    int h = qMax(metrics.lineSpacing(), 15) + 2;
    int w = qMax(metrics.maxWidth(), 10);

    this->ui->tableView->verticalHeader()->hide();
    this->ui->tableView->horizontalHeader()->hide();
    this->ui->tableView->verticalHeader()->setDefaultSectionSize(h);
    this->ui->tableView->horizontalHeader()->setDefaultSectionSize(w);
    this->ui->tableView->verticalHeader()->show();
    this->ui->tableView->horizontalHeader()->show();

    QString strHeight = tr("Real height: %1").arg(metrics.height());
    this->ui->labelRealHeight->setText(strHeight);
}
//-----------------------------------------------------------------------------
QString DialogFontSelect::appendCharacters(const QString &original, const QString &value)
{
    QString result = original;

    for (int i = 0; i < value.length(); i++)
    {
        QChar c = value.at(i);
        if (!result.contains(c))
        {
            result.append(c);
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
QString DialogFontSelect::editorText()
{
    return FontHelper::unescapeControlChars(this->ui->lineEdit->text());
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setEditorText(const QString &value)
{
    this->ui->lineEdit->setText(FontHelper::escapeControlChars(value));
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_fontComboBox_currentFontChanged(const QFont &font)
{
    this->mFontFamily = font.family();

    this->updateStyles();
    this->updateSizes();

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_comboBoxSize_currentIndexChanged(const QString &text)
{
    bool ok;
    int a = text.toInt(&ok);
    if (ok)
        this->mSize = a;
    else
        this->mSize = 5;

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_comboBoxSize_editTextChanged(const QString &text)
{
    this->on_comboBoxSize_currentIndexChanged(text);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_comboBoxStyle_currentIndexChanged(const QString &text)
{
    this->mFontStyle = text;

    this->updateSizes();

    this->applyFont();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_radioButtonProportional_toggled(bool value)
{
    this->mMonospaced = !value;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_checkBoxAntialiasing_toggled(bool value)
{
    this->mAntialiasing = value;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_checkBoxAlphaChannel_toggled(bool value)
{
    this->mAlphaChannel = value;
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_lineEdit_textChanged()
{
    this->mCharacters = this->editorText();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString str = this->editorText();
    QString a = this->mModel->data(index, Qt::DisplayRole).toString();
    this->setEditorText(str + a);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_pushButtonAppendSelected_clicked()
{
    QString selected = QString();

    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndexList indexes = selectionModel->selectedIndexes();
        for (int i = 0; i < indexes.count(); i++)
        {
            QString a = this->mModel->data(indexes.at(i), Qt::DisplayRole).toString();
            selected += a;
        }
    }

    QString original = this->editorText();
    QString result = this->appendCharacters(original, selected);
    this->setEditorText(result);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_pushButtonAppendRange_clicked()
{
    DialogFontRange dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString selected = dialog.resultString();
        QString original = this->editorText();
        QString result = this->appendCharacters(original, selected);
        this->setEditorText(result);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    QString str = this->editorText();
    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    bool hasNew = false;
    if (selectionModel->hasSelection())
    {
        QModelIndexList indexes = selectionModel->selectedIndexes();
        for (int i = 0; i < indexes.count(); i++)
        {
            QString a = this->mModel->data(indexes.at(i), Qt::DisplayRole).toString();
            if (!str.contains(a))
            {
                hasNew = true;
                break;
            }
        }
        this->ui->pushButtonAppendSelected->setEnabled(hasNew);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::rangeChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();

    if (indexes.length() > 0)
    {
        QAbstractItemModel *model = this->ui->listViewBlocks->model();
        bool ok;
        quint32 first = model->data(indexes.at(0), UnicodeBlocksModel::FirstCodeRole).toUInt(&ok);
        quint32 last = model->data(indexes.at(0), UnicodeBlocksModel::LastCodeRole).toUInt(&ok);

        this->mModel->setCodesRange(first, last);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_lineEditUnicodeBlocksFilter_textChanged(const QString &text)
{
    this->mBlocksFilterModel->setNameFilter(text);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_pushButtonSort_clicked()
{
    QString chars = this->editorText();
    QList<QChar> list;
    for (int i = 0; i < chars.length(); i++)
    {
        list.append(chars.at(i));
    }
    qSort(list);
    chars = QString();

    if (!this->mSortOrderUp)
    {
        for (int i = 0; i < list.length(); i++)
        {
            chars.append(list.at(i));
        }
    }
    else
    {
        for (int i = list.length() - 1; i >= 0; i--)
        {
            chars.append(list.at(i));
        }
    }

    this->mSortOrderUp = !this->mSortOrderUp;
    this->setEditorText(chars);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_toolButtonForeColor_clicked()
{
    QColorDialog dialog(FontEditorOptions::foreColor(), this);

    if (dialog.exec() == QDialog::Accepted)
    {
        FontEditorOptions::setForeColor(dialog.selectedColor());
        QPixmap pixmapForeColor = QPixmap(24, 24);
        pixmapForeColor.fill(dialog.selectedColor());
        this->ui->toolButtonForeColor->setIcon(QIcon(pixmapForeColor));
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_toolButtonBackColor_clicked()
{
    QColorDialog dialog(FontEditorOptions::backColor(), this);

    if (dialog.exec() == QDialog::Accepted)
    {
        FontEditorOptions::setBackColor(dialog.selectedColor());
        QPixmap pixmapBackColor = QPixmap(24, 24);
        pixmapBackColor.fill(dialog.selectedColor());
        this->ui->toolButtonBackColor->setIcon(QIcon(pixmapBackColor));
    }
}
//-----------------------------------------------------------------------------
