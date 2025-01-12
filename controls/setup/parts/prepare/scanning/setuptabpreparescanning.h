#ifndef SETUPTABPREPARESCANNING_H
#define SETUPTABPREPARESCANNING_H

#include "conversion_options.h"

#include <QWidget>

namespace Ui
{
class SetupTabPrepareScanning;
}

namespace Settings
{
namespace Presets
{
class Preset;
}
} // namespace Settings

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Prepare
{
class DemoGenerator;

class SetupTabPrepareScanning : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabPrepareScanning(Settings::Presets::Preset* preset, QWidget* parent = 0);
  virtual ~SetupTabPrepareScanning();

public slots:
  void matrixChanged();

private:
  Ui::SetupTabPrepareScanning* ui;
  Settings::Presets::Preset* mPreset;
  QPixmap mPixmapScanning;
  QPixmap mPixmapScanPreview;
  DemoGenerator* mDemoGen;

private slots:
  void on_comboBoxScanMain_currentIndexChanged(int index);
  void on_comboBoxScanSub_currentIndexChanged(int index);
  void on_checkBoxBands_toggled(bool value);
  void on_spinBoxBandWidth_valueChanged(int value);
  void on_checkBoxUseCustomScript_toggled(bool value);
  void on_plainTextEditCustomScript_textChanged();
  void updateState();
  void updateScript();
  void demoPixmapChanged(const QPixmap& pixmap);
  void demoScriptError(const QString& value);
  void on_spinBoxAnimationTime_valueChanged(int value);
  void on_spinBoxAnimationInterval_valueChanged(int value);
};

} // namespace Prepare
} // namespace Parts
} // namespace Setup
} // namespace AppUI

#endif // SETUPTABPREPARESCANNING_H
