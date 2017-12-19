#ifndef SETUPTABPREPAREPREPROCESSING_H
#define SETUPTABPREPAREPREPROCESSING_H

#include <QWidget>

#include "conversion_options.h"

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
}

class SetupTabPreparePreprocessing : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabPreparePreprocessing(Settings::Presets::Preset *preset, QWidget *parent = 0);
  virtual ~SetupTabPreparePreprocessing();

public slots:
  void matrixChanged();

private:
  Ui::SetupTabPreparePreprocessing *ui;
  Settings::Presets::Preset *mPreset;
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

#endif // SETUPTABPREPAREPREPROCESSING_H
