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

    QString codec = this->mPreset->font()->codec();

    this->ui->comboBoxCodec->addItems(FontOptions::codecs());

    int index = this->ui->comboBoxCodec->findData(codec);
    if (index >= 0)
        this->ui->comboBoxCodec->setCurrentIndex(index);


    this->matrixChanged();
}
//-----------------------------------------------------------------------------
SetupTabFont::~SetupTabFont()
{
    delete ui;
}
//-----------------------------------------------------------------------------
const QString &SetupTabFont::title()
{
    static const QString result = QObject::tr("Font");
    return result;
}
//-----------------------------------------------------------------------------
void SetupTabFont::matrixChanged()
{
    int index = this->ui->comboBoxCodec->findText(this->mPreset->font()->codec());
    if (index >= 0)
        this->ui->comboBoxCodec->setCurrentIndex(index);

    this->ui->checkBoxBom->setChecked(this->mPreset->font()->bom());
}
//-----------------------------------------------------------------------------
void SetupTabFont::on_checkBoxBom_toggled(bool value)
{
    this->mPreset->font()->setBom(value);
}
//-----------------------------------------------------------------------------
void SetupTabFont::on_comboBoxCodec_currentIndexChanged(const QString &value)
{
    this->mPreset->font()->setCodec(value);
}
//-----------------------------------------------------------------------------
