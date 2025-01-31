#ifndef SETUPTABPREPAREPREPROCESSING_H
#define SETUPTABPREPAREPREPROCESSING_H

#include "conversion_options.h"

#include <QWidget>

namespace Ui
{
class SetupTabPreparePreprocessing;
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

class SetupTabPreparePreprocessing : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabPreparePreprocessing(Settings::Presets::Preset* preset, QWidget* parent = 0);
  virtual ~SetupTabPreparePreprocessing();

public slots:
  void matrixChanged();

private:
  Ui::SetupTabPreparePreprocessing* ui;
  Settings::Presets::Preset* mPreset;
  QPixmap mPixmapScanning;
  QPixmap mPixmapScanPreview;

private slots:
  void on_comboBoxConversionType_currentIndexChanged(int index);
  void on_comboBoxMonochromeType_currentIndexChanged(int index);
  void on_checkBoxInverse_toggled(bool value);
  void on_horizontalScrollBarEdge_valueChanged(int value);
  void on_plainTextEditCustomScript_textChanged();
  void on_lineEditDemoInput_textChanged();
  void updateState();
  void updateScript();
};

} // namespace Prepare
} // namespace Parts
} // namespace Setup
} // namespace AppUI

#endif // SETUPTABPREPAREPREPROCESSING_H
