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
#include <QSettings>
#include "charactersmodel.h"
#include "unicodeblocksmodel.h"
#include "unicodeblocksfiltermodel.h"
#include "dialogfontrange.h"
#include "fonthelper.h"
#include "fonteditoroptions.h"
#include "tfontparameters.h"
#include "dialogfontselectdata.h"
//-----------------------------------------------------------------------------
DialogFontSelect::DialogFontSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontSelect)
{
    ui->setupUi(this);

    this->mData = new DialogFontSelectData(this);

    this->mData->charactersModel()->setCodesRange(0x0000, 0x00ff);
    this->ui->tableView->setModel(this->mData->charactersModel());

    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    this->ui->radioButtonMonospaced->setChecked(false);
    this->ui->radioButtonProportional->setChecked(false);
    this->ui->checkBoxAntialiasing->setChecked(false);

    this->ui->listViewBlocks->setModel(this->mData->unicodeBlocksModel());

    selectionModel = this->ui->listViewBlocks->selectionModel();
    this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this->mData, SLOT(setUnicodeRange(QItemSelection,QItemSelection)));

    // Set font
    this->connect(this->ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this->mData, SLOT(setFont(QFont)));
    // Get styles
    this->connect(this->mData, SIGNAL(stylesListChanged(QStringList,QString)), SLOT(on_stylesListChanged(QStringList,QString)));
    // Set style
    this->connect(this->ui->comboBoxStyle, SIGNAL(currentIndexChanged(QString)), this->mData, SLOT(setStyle(QString)));
    // Get sizes
    this->connect(this->mData, SIGNAL(sizesListChanged(QList<int>,int)), SLOT(on_sizesListChanged(QList<int>,int)));
    // Set size
    this->connect(this->ui->comboBoxSize, SIGNAL(currentIndexChanged(QString)), this->mData, SLOT(setSize(QString)));
    this->connect(this->ui->comboBoxSize, SIGNAL(currentTextChanged(QString)), this->mData, SLOT(setSize(QString)));
    // Set unicode blocks filter
    this->connect(this->ui->lineEditUnicodeBlocksFilter, SIGNAL(textChanged(QString)), this->mData, SLOT(setUnicodeBlocksFilter(QString)));

    // Character's line edit
    this->connect(this->ui->lineEdit, SIGNAL(textChanged(QString)), SLOT(on_lineEdit_textChanged(QString)));
    this->connect(this->mData, SIGNAL(charactersListChanged(QString)), SLOT(on_charactersListChanged(QString)));

    // Apply changed font to character's table
    this->connect(this->mData, SIGNAL(fontChanged(QFont)), SLOT(on_fontChanged(QFont)));
    this->connect(this->mData, SIGNAL(fontMeasured(int,int,int)), SLOT(on_fontMeasured(int,int,int)));

    // Checkbox/RadioButton
    this->connect(this->ui->radioButtonMonospaced, SIGNAL(toggled(bool)), this->mData, SLOT(setMonospaced(bool)));
    this->connect(this->ui->checkBoxAntialiasing, SIGNAL(toggled(bool)), this->mData, SLOT(setAntialiasing(bool)));
    this->connect(this->mData, SIGNAL(antialiasingChanged(bool)), this->ui->checkBoxAntialiasing, SLOT(setChecked(bool)));
    this->connect(this->mData, SIGNAL(monospacedChanged(bool)), SLOT(on_monospacedChanged(bool)));

    // Colors
    this->connect(this->mData, SIGNAL(colorsChanged(QColor,QColor)), SLOT(updateColorIcons(QColor,QColor)));

    // Sort characters
    this->connect(this->ui->pushButtonSort, SIGNAL(clicked(bool)), this->mData, SLOT(resort()));

    this->updateColorIcons(FontEditorOptions::foreColor(), FontEditorOptions::backColor());

    this->mData->setFont(this->ui->fontComboBox->currentFont());
    this->ui->lineEdit->setText(this->mData->characters());

    this->loadSettings();
}
//-----------------------------------------------------------------------------
DialogFontSelect::~DialogFontSelect()
{
    this->saveSettings();
    delete ui;
}
//-----------------------------------------------------------------------------
QString DialogFontSelect::characters()
{
    return this->mData->characters();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setCharacters(const QString &value)
{
    this->mData->setCharacters(value);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::getFontParameters(tFontParameters *parameters)
{
    this->mData->getFontParameters(parameters);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::setFontParameters(const tFontParameters &parameters)
{
    this->mData->setFontParameters(parameters);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::loadSettings()
{
    QSettings sett;
    sett.beginGroup("dialog-font-select");
    sett.beginGroup("toolbox");

    bool ok;
    int pageIndex = sett.value("pageIndex", QVariant("none")).toUInt(&ok);

    if (ok)
    {
        if (pageIndex < this->ui->toolBox->count())
        {
            this->ui->toolBox->setCurrentIndex(pageIndex);
        }
    }

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::saveSettings() const
{
    QSettings sett;
    sett.beginGroup("dialog-font-select");
    sett.beginGroup("toolbox");

    int pageIndex = this->ui->toolBox->currentIndex();
    sett.setValue("pageIndex", QVariant(pageIndex));

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_lineEdit_textChanged(const QString &value)
{
    QString stringNew = FontHelper::unescapeControlChars(value);
    QString stringOld = this->mData->characters();

    if (stringNew != stringOld)
    {
        this->mData->setCharacters(stringNew);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_tableView_doubleClicked(const QModelIndex &index)
{
    QString selected = this->ui->tableView->model()->data(index, Qt::DisplayRole).toString();
    this->mData->appendCharacters(selected);
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
            QString a = this->ui->tableView->model()->data(indexes.at(i), Qt::DisplayRole).toString();
            selected += a;
        }
    }

    this->mData->appendCharacters(selected);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_pushButtonAppendRange_clicked()
{
    DialogFontRange dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString selected = dialog.resultString();
        this->mData->appendCharacters(selected);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    QString str = this->mData->characters();
    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    bool hasNew = false;

    if (selectionModel->hasSelection())
    {
        QModelIndexList indexes = selectionModel->selectedIndexes();

        for (int i = 0; i < indexes.count(); i++)
        {
            QString a = this->ui->tableView->model()->data(indexes.at(i), Qt::DisplayRole).toString();

            if (!str.contains(a))
            {
                hasNew = true;
                break;
            }
        }
    }

    this->ui->pushButtonAppendSelected->setEnabled(hasNew);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_pushButtonForeColor_clicked()
{
    QColorDialog dialog(this->mData->foreground(), this);
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);

    if (dialog.exec() == QDialog::Accepted)
    {
        FontEditorOptions::setForeColor(dialog.selectedColor());
        this->mData->setForeground(dialog.selectedColor());
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_pushButtonBackColor_clicked()
{
    QColorDialog dialog(this->mData->background(), this);
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);

    if (dialog.exec() == QDialog::Accepted)
    {
        FontEditorOptions::setBackColor(dialog.selectedColor());
        this->mData->setBackground(dialog.selectedColor());
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_stylesListChanged(const QStringList &list, const QString &selected)
{
    this->ui->comboBoxStyle->clear();
    this->ui->comboBoxStyle->addItems(list);

    int index = list.indexOf(selected);

    if (index >= 0)
        this->ui->comboBoxStyle->setCurrentIndex(index);
    else
        this->ui->comboBoxStyle->setCurrentIndex(0);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_sizesListChanged(const QList<int> &list, int selected)
{
    int i = 0, previousSizeIndex = 0;
    QListIterator<int> it(list);
    it.toFront();
    this->ui->comboBoxSize->clear();

    while (it.hasNext())
    {
        int size = it.next();

        this->ui->comboBoxSize->addItem(QString("%1").arg(size), QVariant(size));

        if (selected == 14 && previousSizeIndex == 0)
            previousSizeIndex = i;

        if (size == selected)
            previousSizeIndex = i;

        i++;
    }

    this->ui->comboBoxSize->setCurrentIndex(previousSizeIndex);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_charactersListChanged(const QString &value)
{
    QString stringNew = FontHelper::escapeControlChars(value);
    QString stringOld = this->ui->lineEdit->text();

    if (stringOld != stringNew)
    {
        this->ui->lineEdit->setText(stringNew);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_fontChanged(const QFont &value)
{
    this->ui->tableView->setFont(value);

    QFontMetrics metrics(value);

    int h = qMax(metrics.lineSpacing(), 15) + 2;
    int w = qMax(metrics.maxWidth(), 10);

    this->ui->tableView->verticalHeader()->hide();
    this->ui->tableView->horizontalHeader()->hide();
    this->ui->tableView->verticalHeader()->setDefaultSectionSize(h);
    this->ui->tableView->horizontalHeader()->setDefaultSectionSize(w);
    this->ui->tableView->verticalHeader()->show();
    this->ui->tableView->horizontalHeader()->show();

    QFont currentFont = this->ui->fontComboBox->currentFont();

    if ((currentFont.family() != value.family()))
    {
        this->ui->fontComboBox->setCurrentFont(value);
    }
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_monospacedChanged(bool value)
{
    this->ui->radioButtonMonospaced->setChecked(value);
    this->ui->radioButtonProportional->setChecked(!value);
}
//-----------------------------------------------------------------------------
void DialogFontSelect::on_fontMeasured(int count, int maxWidth, int maxHeight)
{
    this->ui->labelCharactersMaxSize->setText(tr("Max size (w × h): %1 × %2").arg(maxWidth).arg(maxHeight));
    this->ui->labelCharactersCount->setText(tr("Count: %1").arg(count));
}
//-----------------------------------------------------------------------------
void DialogFontSelect::updateColorIcons(const QColor &foreground, const QColor &background)
{
    QPixmap pixmapForeColor = QPixmap(24, 24);
    pixmapForeColor.fill(foreground);
    this->ui->pushButtonForeColor->setIcon(QIcon(pixmapForeColor));
    this->ui->pushButtonForeColor->setText(tr("Fore Color: %1").arg((quint32)foreground.rgba(), 8, 16, QChar('0')));

    QPixmap pixmapBackColor = QPixmap(24, 24);
    pixmapBackColor.fill(background);
    this->ui->pushButtonBackColor->setIcon(QIcon(pixmapBackColor));
    this->ui->pushButtonBackColor->setText(tr("Back Color: %1").arg((quint32)background.rgba(), 8, 16, QChar('0')));
}
//-----------------------------------------------------------------------------
