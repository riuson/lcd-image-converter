#include "setuptabpreparepreprocessing.h"

#include "ui_setuptabpreparepreprocessing.h"

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

SetupTabPreparePreprocessing::SetupTabPreparePreprocessing(Settings::Presets::Preset* preset, QWidget* parent)
    : QWidget(parent), ui(new Ui::SetupTabPreparePreprocessing)
{
  ui->setupUi(this);
  this->mPreset = preset;
  this->mPixmapScanning = QPixmap();
  this->mPixmapScanPreview = QPixmap();

  this->ui->comboBoxConversionType->addItem(tr("Monochrome"),
                                            static_cast<int>(Parsing::Conversion::Options::ConversionType::Monochrome));
  this->ui->comboBoxConversionType->addItem(tr("Grayscale"),
                                            static_cast<int>(Parsing::Conversion::Options::ConversionType::Grayscale));
  this->ui->comboBoxConversionType->addItem(tr("Color"),
                                            static_cast<int>(Parsing::Conversion::Options::ConversionType::Color));
  this->ui->comboBoxConversionType->addItem(tr("Custom"),
                                            static_cast<int>(Parsing::Conversion::Options::ConversionType::Custom));

  this->ui->comboBoxMonochromeType->addItem(tr("Edge"),
                                            static_cast<int>(Parsing::Conversion::Options::MonochromeType::Edge));
  this->ui->comboBoxMonochromeType->addItem(
      QString("Diffuse Dither"), static_cast<int>(Parsing::Conversion::Options::MonochromeType::DiffuseDither));
  this->ui->comboBoxMonochromeType->addItem(
      QString("Ordered Dither"), static_cast<int>(Parsing::Conversion::Options::MonochromeType::OrderedDither));
  this->ui->comboBoxMonochromeType->addItem(
      QString("Threshold Dither"), static_cast<int>(Parsing::Conversion::Options::MonochromeType::ThresholdDither));

  this->matrixChanged();
}

SetupTabPreparePreprocessing::~SetupTabPreparePreprocessing() { delete ui; }

void SetupTabPreparePreprocessing::matrixChanged()
{
  int index = this->ui->comboBoxConversionType->findData(static_cast<int>(this->mPreset->prepare()->convType()));

  if (index >= 0) {
    this->ui->comboBoxConversionType->setCurrentIndex(index);
  }

  index = this->ui->comboBoxMonochromeType->findData(static_cast<int>(this->mPreset->prepare()->monoType()));

  if (index >= 0) {
    this->ui->comboBoxMonochromeType->setCurrentIndex(index);
  }

  this->ui->horizontalScrollBarEdge->setValue(this->mPreset->prepare()->edge());

  this->ui->checkBoxInverse->setChecked(this->mPreset->prepare()->inverse());

  this->updateState();

  this->updateScript();
}

void SetupTabPreparePreprocessing::on_comboBoxConversionType_currentIndexChanged(int index)
{
  QVariant data = this->ui->comboBoxConversionType->itemData(index);
  bool ok;
  int a = data.toInt(&ok);

  if (ok) {
    this->mPreset->prepare()->setConvType((Settings::Presets::ConversionType)a);
    this->updateState();
  }
}

void SetupTabPreparePreprocessing::on_comboBoxMonochromeType_currentIndexChanged(int index)
{
  QVariant data = this->ui->comboBoxMonochromeType->itemData(index);
  bool ok;
  int a = data.toInt(&ok);

  if (ok) {
    this->mPreset->prepare()->setMonoType((Settings::Presets::MonochromeType)a);
    this->updateState();
  }
}

void SetupTabPreparePreprocessing::on_checkBoxInverse_toggled(bool value)
{
  this->mPreset->prepare()->setInverse(value);
}

void SetupTabPreparePreprocessing::on_horizontalScrollBarEdge_valueChanged(int value)
{
  this->mPreset->prepare()->setEdge(value);
}

void SetupTabPreparePreprocessing::on_plainTextEditCustomScript_textChanged()
{
  if (this->mPreset->prepare()->convType() == Parsing::Conversion::Options::ConversionType::Custom) {
    QString str = this->ui->plainTextEditCustomScript->toPlainText();

    QVector<quint32> data;
    data.append(0);
    QString scriptError;
    Parsing::Conversion::ConverterHelper::convertPixelsByScript(str, &data, &scriptError);

    if (scriptError.isEmpty()) {
      this->mPreset->prepare()->setCustomPreprocessScript(str);
      this->ui->labelErrorMessage->setText(QString());
      this->on_lineEditDemoInput_textChanged();
    } else {
      this->ui->labelErrorMessage->setText(scriptError);
    }
  }
}

void SetupTabPreparePreprocessing::on_lineEditDemoInput_textChanged()
{
  if (this->mPreset->prepare()->convType() == Parsing::Conversion::Options::ConversionType::Custom) {
    QString demoInputString = this->ui->lineEditDemoInput->text();
    quint32 demoInputValue;
    bool ok = false;

    if (demoInputString.contains(QChar('x'), Qt::CaseInsensitive)) {
      demoInputValue = demoInputString.toULong(&ok, 16);
    } else {
      demoInputValue = demoInputString.toULong(&ok, 10);
    }

    if (ok) {
      QString script = this->ui->plainTextEditCustomScript->toPlainText();

      QVector<quint32> demoData;
      demoData.append(demoInputValue);
      QString scriptError;
      Parsing::Conversion::ConverterHelper::convertPixelsByScript(script, &demoData, &scriptError);

      if (scriptError.isEmpty()) {
        this->mPreset->prepare()->setCustomPreprocessScript(script);
        this->ui->labelErrorMessage->setText(QString());
      } else {
        this->ui->labelErrorMessage->setText(scriptError);
      }

      quint32 demoOutputValue = demoData.at(0);
      this->ui->lineEditDemoOutput->setText(QString("0x%1").arg(demoOutputValue, 8, 16, QChar('0')));
    }
  }
}

void SetupTabPreparePreprocessing::updateState()
{
  // conversion type
  if (this->mPreset->prepare()->convType() == Parsing::Conversion::Options::ConversionType::Monochrome) {
    this->ui->comboBoxMonochromeType->setEnabled(true);

    if (this->mPreset->prepare()->monoType() == Parsing::Conversion::Options::MonochromeType::Edge) {
      this->ui->horizontalScrollBarEdge->setEnabled(true);
    } else {
      this->ui->horizontalScrollBarEdge->setEnabled(false);
    }
  } else {
    this->ui->comboBoxMonochromeType->setEnabled(false);
    this->ui->horizontalScrollBarEdge->setEnabled(false);
  }

  // monochrome type
  if (this->mPreset->prepare()->convType() == Parsing::Conversion::Options::ConversionType::Monochrome) {
    this->ui->comboBoxMonochromeType->setEnabled(true);

    if (this->mPreset->prepare()->monoType() == Parsing::Conversion::Options::MonochromeType::Edge) {
      this->ui->horizontalScrollBarEdge->setEnabled(true);
    } else {
      this->ui->horizontalScrollBarEdge->setEnabled(false);
    }
  } else {
    this->ui->comboBoxMonochromeType->setEnabled(false);
    this->ui->horizontalScrollBarEdge->setEnabled(false);
  }

  this->ui->plainTextEditCustomScript->setReadOnly(this->mPreset->prepare()->convType() !=
                                                   Parsing::Conversion::Options::ConversionType::Custom);
}

void SetupTabPreparePreprocessing::updateScript()
{
  QString script = Parsing::Conversion::ConverterHelper::pixelsScript(this->mPreset);

  if (this->ui->plainTextEditCustomScript->toPlainText() != script) {
    this->ui->plainTextEditCustomScript->setPlainText(script);
  }
}

} // namespace Prepare
} // namespace Parts
} // namespace Setup
} // namespace AppUI
