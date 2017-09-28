#include "setuptabpreparescanning.h"
#include "ui_setuptabpreparescanning.h"

#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
#include "bitmaphelper.h"
#include "converterhelper.h"
#include "demogenerator.h"
#include "setupdialogoptions.h"

SetupTabPrepareScanning::SetupTabPrepareScanning(Preset *preset, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SetupTabPrepareScanning)
{
  ui->setupUi(this);
  this->mPreset = preset;
  this->mPixmapScanning = QPixmap();
  this->mPixmapScanPreview = QPixmap();
  this->mDemoGen = new DemoGenerator(this->mPreset, this);
  this->connect(this->mDemoGen, SIGNAL(pixmapChanged(const QPixmap &)), SLOT(demoPixmapChanged(const QPixmap &)));
  this->connect(this->mDemoGen, SIGNAL(errorHandled(QString)), SLOT(demoScriptError(QString)));

  this->ui->comboBoxScanMain->addItem(tr("Top to Bottom"), QVariant(TopToBottom));
  this->ui->comboBoxScanMain->addItem(tr("Bottom to Top"), QVariant(BottomToTop));
  this->ui->comboBoxScanMain->addItem(tr("Left to Right"), QVariant(LeftToRight));
  this->ui->comboBoxScanMain->addItem(tr("Right to Left"), QVariant(RightToLeft));

  this->ui->comboBoxScanSub->addItem(tr("Forward"), QVariant(Forward));
  this->ui->comboBoxScanSub->addItem(tr("Backward"), QVariant(Backward));

  this->ui->spinBoxAnimationTime->setValue(SetupDialogOptions::animationTotalTime());
  this->ui->spinBoxAnimationInterval->setValue(SetupDialogOptions::animationInterval());

  this->matrixChanged();
}

SetupTabPrepareScanning::~SetupTabPrepareScanning()
{
  delete ui;
}

void SetupTabPrepareScanning::matrixChanged()
{
  int index = this->ui->comboBoxScanMain->findData(this->mPreset->prepare()->scanMain());

  if (index >= 0) {
    this->ui->comboBoxScanMain->setCurrentIndex(index);
  }

  index = this->ui->comboBoxScanSub->findData(this->mPreset->prepare()->scanSub());

  if (index >= 0) {
    this->ui->comboBoxScanSub->setCurrentIndex(index);
  }

  this->ui->checkBoxBands->setChecked(this->mPreset->prepare()->bandScanning());

  this->ui->spinBoxBandWidth->setValue(this->mPreset->prepare()->bandWidth());

  this->ui->checkBoxUseCustomScript->setChecked(this->mPreset->prepare()->useCustomScript());

  this->updateState();

  this->updateScript();
}

void SetupTabPrepareScanning::on_comboBoxScanMain_currentIndexChanged(int index)
{
  QVariant data = this->ui->comboBoxScanMain->itemData(index);
  bool ok;
  int a = data.toInt(&ok);

  if (ok) {
    ScanMainDirection dir = (ScanMainDirection)a;
    this->mPreset->prepare()->setScanMain(dir);
  }
}

void SetupTabPrepareScanning::on_comboBoxScanSub_currentIndexChanged(int index)
{
  QVariant data = this->ui->comboBoxScanSub->itemData(index);
  bool ok;
  int a = data.toInt(&ok);

  if (ok) {
    ScanSubDirection dir = (ScanSubDirection)a;
    this->mPreset->prepare()->setScanSub(dir);
  }
}

void SetupTabPrepareScanning::on_checkBoxBands_toggled(bool value)
{
  this->mPreset->prepare()->setBandScanning(value);
  this->updateState();
}

void SetupTabPrepareScanning::on_spinBoxBandWidth_valueChanged(int value)
{
  this->mPreset->prepare()->setBandWidth(value);
}

void SetupTabPrepareScanning::on_checkBoxUseCustomScript_toggled(bool value)
{
  this->mPreset->prepare()->setUseCustomScript(value);
  this->updateState();
}

void SetupTabPrepareScanning::on_plainTextEditCustomScript_textChanged()
{
  if (this->mPreset->prepare()->useCustomScript()) {
    QString str = this->ui->plainTextEditCustomScript->toPlainText();
    this->mPreset->prepare()->setCustomScript(str);
  }
}

void SetupTabPrepareScanning::updateState()
{
  // band scan
  this->ui->spinBoxBandWidth->setEnabled(this->mPreset->prepare()->bandScanning());

  // use custom script
  this->ui->plainTextEditCustomScript->setReadOnly(!this->mPreset->prepare()->useCustomScript());
}

void SetupTabPrepareScanning::updateScript()
{
  QString script = ConverterHelper::scanScript(this->mPreset);

  if (this->ui->plainTextEditCustomScript->toPlainText() != script) {
    this->ui->plainTextEditCustomScript->setPlainText(script);
  }

  this->mDemoGen->setScript(script);
}

void SetupTabPrepareScanning::demoPixmapChanged(const QPixmap &pixmap)
{
  this->mPixmapScanPreview = pixmap;

  this->ui->labelScanPreview->setPixmap(this->mPixmapScanPreview);
  this->ui->spinBoxAnimationTime->setEnabled(true);
  this->ui->spinBoxAnimationInterval->setEnabled(true);

  this->ui->labelErrorMessage->hide();
}

void SetupTabPrepareScanning::demoScriptError(const QString &value)
{
  this->ui->spinBoxAnimationTime->setEnabled(false);
  this->ui->spinBoxAnimationInterval->setEnabled(false);

  this->ui->labelErrorMessage->setText(value);
  this->ui->labelErrorMessage->show();
}

void SetupTabPrepareScanning::on_spinBoxAnimationTime_valueChanged(int value)
{
  this->mDemoGen->setAnimationTime(value);
  SetupDialogOptions::setAnimationTime(value);
}

void SetupTabPrepareScanning::on_spinBoxAnimationInterval_valueChanged(int value)
{
  this->mDemoGen->setAnimationInterval(value);
  SetupDialogOptions::setAnimationInterval(value);
}
