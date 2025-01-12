#include "setuptabpreparescanning.h"

#include "ui_setuptabpreparescanning.h"

#include "bitmaphelper.h"
#include "converterhelper.h"
#include "demogenerator.h"
#include "imageoptions.h"
#include "matrixoptions.h"
#include "prepareoptions.h"
#include "preset.h"
#include "setupdialogoptions.h"

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Prepare
{

SetupTabPrepareScanning::SetupTabPrepareScanning(Settings::Presets::Preset* preset, QWidget* parent)
    : QWidget(parent), ui(new Ui::SetupTabPrepareScanning)
{
  ui->setupUi(this);
  this->mPreset = preset;
  this->mPixmapScanning = QPixmap();
  this->mPixmapScanPreview = QPixmap();
  this->mDemoGen = new DemoGenerator(this->mPreset, this);
  this->connect(this->mDemoGen, SIGNAL(pixmapChanged(const QPixmap&)), SLOT(demoPixmapChanged(const QPixmap&)));
  this->connect(this->mDemoGen, SIGNAL(errorHandled(QString)), SLOT(demoScriptError(QString)));

  this->ui->comboBoxScanMain->addItem(
      tr("Top to Bottom"), QVariant(static_cast<int>(Parsing::Conversion::Options::ScanMainDirection::TopToBottom)));
  this->ui->comboBoxScanMain->addItem(
      tr("Bottom to Top"), QVariant(static_cast<int>(Parsing::Conversion::Options::ScanMainDirection::BottomToTop)));
  this->ui->comboBoxScanMain->addItem(
      tr("Left to Right"), QVariant(static_cast<int>(Parsing::Conversion::Options::ScanMainDirection::LeftToRight)));
  this->ui->comboBoxScanMain->addItem(
      tr("Right to Left"), QVariant(static_cast<int>(Parsing::Conversion::Options::ScanMainDirection::RightToLeft)));

  this->ui->comboBoxScanSub->addItem(
      tr("Forward"), QVariant(static_cast<int>(Parsing::Conversion::Options::ScanSubDirection::Forward)));
  this->ui->comboBoxScanSub->addItem(
      tr("Backward"), QVariant(static_cast<int>(Parsing::Conversion::Options::ScanSubDirection::Backward)));

  this->ui->spinBoxAnimationTime->setValue(Settings::SetupDialogOptions::animationTotalTime());
  this->ui->spinBoxAnimationInterval->setValue(Settings::SetupDialogOptions::animationInterval());

  this->matrixChanged();
}

SetupTabPrepareScanning::~SetupTabPrepareScanning() { delete ui; }

void SetupTabPrepareScanning::matrixChanged()
{
  int index = this->ui->comboBoxScanMain->findData(static_cast<int>(this->mPreset->prepare()->scanMain()));

  if (index >= 0) {
    this->ui->comboBoxScanMain->setCurrentIndex(index);
  }

  index = this->ui->comboBoxScanSub->findData(static_cast<int>(this->mPreset->prepare()->scanSub()));

  if (index >= 0) {
    this->ui->comboBoxScanSub->setCurrentIndex(index);
  }

  this->ui->checkBoxBands->setChecked(this->mPreset->prepare()->bandScanning());

  this->ui->spinBoxBandWidth->setValue(this->mPreset->prepare()->bandWidth());

  this->ui->checkBoxUseCustomScript->setChecked(this->mPreset->prepare()->useCustomScanScript());

  this->updateState();

  this->updateScript();
}

void SetupTabPrepareScanning::on_comboBoxScanMain_currentIndexChanged(int index)
{
  QVariant data = this->ui->comboBoxScanMain->itemData(index);
  bool ok;
  int a = data.toInt(&ok);

  if (ok) {
    Settings::Presets::ScanMainDirection dir = (Settings::Presets::ScanMainDirection)a;
    this->mPreset->prepare()->setScanMain(dir);
    this->updateScript();
  }
}

void SetupTabPrepareScanning::on_comboBoxScanSub_currentIndexChanged(int index)
{
  QVariant data = this->ui->comboBoxScanSub->itemData(index);
  bool ok;
  int a = data.toInt(&ok);

  if (ok) {
    Settings::Presets::ScanSubDirection dir = (Settings::Presets::ScanSubDirection)a;
    this->mPreset->prepare()->setScanSub(dir);
    this->updateScript();
  }
}

void SetupTabPrepareScanning::on_checkBoxBands_toggled(bool value)
{
  this->mPreset->prepare()->setBandScanning(value);
  this->updateState();
  this->updateScript();
}

void SetupTabPrepareScanning::on_spinBoxBandWidth_valueChanged(int value)
{
  this->mPreset->prepare()->setBandWidth(value);
}

void SetupTabPrepareScanning::on_checkBoxUseCustomScript_toggled(bool value)
{
  this->mPreset->prepare()->setUseCustomScanScript(value);
  this->updateState();
  this->updateScript();
}

void SetupTabPrepareScanning::on_plainTextEditCustomScript_textChanged()
{
  if (this->mPreset->prepare()->useCustomScanScript()) {
    QString str = this->ui->plainTextEditCustomScript->toPlainText();
    this->mPreset->prepare()->setCustomScanScript(str);
    this->updateScript();
  }
}

void SetupTabPrepareScanning::updateState()
{
  // band scan
  this->ui->spinBoxBandWidth->setEnabled(this->mPreset->prepare()->bandScanning());

  // use custom script
  this->ui->plainTextEditCustomScript->setReadOnly(!this->mPreset->prepare()->useCustomScanScript());
}

void SetupTabPrepareScanning::updateScript()
{
  QString script = Parsing::Conversion::ConverterHelper::scanScript(this->mPreset);

  if (this->ui->plainTextEditCustomScript->toPlainText() != script) {
    this->ui->plainTextEditCustomScript->setPlainText(script);
  }

  this->mDemoGen->setScript(script);
}

void SetupTabPrepareScanning::demoPixmapChanged(const QPixmap& pixmap)
{
  this->mPixmapScanPreview = pixmap;

  this->ui->labelScanPreview->setPixmap(this->mPixmapScanPreview);
  this->ui->spinBoxAnimationTime->setEnabled(true);
  this->ui->spinBoxAnimationInterval->setEnabled(true);

  this->ui->labelErrorMessage->hide();
}

void SetupTabPrepareScanning::demoScriptError(const QString& value)
{
  this->ui->spinBoxAnimationTime->setEnabled(false);
  this->ui->spinBoxAnimationInterval->setEnabled(false);

  this->ui->labelErrorMessage->setText(value);
  this->ui->labelErrorMessage->show();
}

void SetupTabPrepareScanning::on_spinBoxAnimationTime_valueChanged(int value)
{
  this->mDemoGen->setAnimationTime(value);
  Settings::SetupDialogOptions::setAnimationTime(value);
}

void SetupTabPrepareScanning::on_spinBoxAnimationInterval_valueChanged(int value)
{
  this->mDemoGen->setAnimationInterval(value);
  Settings::SetupDialogOptions::setAnimationInterval(value);
}

} // namespace Prepare
} // namespace Parts
} // namespace Setup
} // namespace AppUI
