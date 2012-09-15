#include "setuptabfont.h"
#include "ui_setuptabfont.h"
//-----------------------------------------------------------------------------
#include "preset.h"
#include "fontoptions.h"
//-----------------------------------------------------------------------------
SetupTabFont::SetupTabFont(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabFont)
{
    ui->setupUi(this);
    this->mPreset = preset;

    QString encoding = this->mPreset->font()->encoding();

    this->ui->comboBoxEncoding->addItems(FontOptions::encodings());

    int index = this->ui->comboBoxEncoding->findData(encoding);
    if (index >= 0)
        this->ui->comboBoxEncoding->setCurrentIndex(index);


    this->matrixChanged();
}
//-----------------------------------------------------------------------------
SetupTabFont::~SetupTabFont()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void SetupTabFont::matrixChanged()
{
    int index = this->ui->comboBoxEncoding->findText(this->mPreset->font()->encoding());
    if (index >= 0)
        this->ui->comboBoxEncoding->setCurrentIndex(index);

    this->ui->checkBoxBom->setChecked(this->mPreset->font()->bom());
}
//-----------------------------------------------------------------------------
void SetupTabFont::on_checkBoxBom_toggled(bool value)
{
    this->mPreset->font()->setBom(value);
}
//-----------------------------------------------------------------------------
void SetupTabFont::on_comboBoxEncoding_currentIndexChanged(const QString &value)
{
    this->mPreset->font()->setEncoding(value);
}
//-----------------------------------------------------------------------------
