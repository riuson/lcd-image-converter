/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2015 riuson
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

#ifndef TOOLZOOM_H
#define TOOLZOOM_H

#include <QObject>
#include "iimageeditortool.h"

#include <QIcon>
#include <QSpinBox>

namespace ImageEditor
{
class IImageEditorParams;

namespace Tools
{

class ToolZoom : public QObject, public IImageEditorTool
{
  Q_OBJECT
  Q_INTERFACES(ImageEditor::Tools::IImageEditorTool)

public:
  explicit ToolZoom(IImageEditorParams *parameters, QObject *parent = 0);
  virtual ~ToolZoom();

  const QString title() const Q_DECL_OVERRIDE;
  const QString tooltip() const Q_DECL_OVERRIDE;
  const QIcon *icon() const Q_DECL_OVERRIDE;
  const QList<QAction *> *actions() const Q_DECL_OVERRIDE;
  const QList<QWidget *> *widgets() const Q_DECL_OVERRIDE;

  int scale() const;

public slots:
  bool processMouse(QMouseEvent *event,
                    const QImage *imageOriginal,
                    bool inRect) Q_DECL_OVERRIDE;
  void setScale(int value);

signals:
  void started(const QImage *value) Q_DECL_OVERRIDE;
  void processing(const QImage *value) Q_DECL_OVERRIDE;
  void completed(const QImage *value, bool changed) Q_DECL_OVERRIDE;
  void scaleChanged(int value);

private:
  IImageEditorParams *mParameters;
  QIcon *mIcon;
  QSpinBox *mSpinBoxScale;
  QList<QAction *> *mActions;
  QList<QWidget *> *mWidgets;
  int mScale;

  void initializeWidgets();
  void loadSettings();
  void saveSettings() const;

private slots:
  void on_spinBoxScale_valueChanged(int value);
};

} // namespace Tools
} // namespace ImageEditor

#endif // TOOLZOOM_H
