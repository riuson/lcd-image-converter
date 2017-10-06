/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#ifndef SETUPTABPREPARE_H
#define SETUPTABPREPARE_H

#include <QWidget>

#include "conversion_options.h"

namespace Ui
{
class SetupTabPrepare;
}

class Preset;
class SetupTabPrepareScanning;
class SetupTabPreparePreprocessing;

using namespace ConversionOptions;

class SetupTabPrepare : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabPrepare(Preset *preset, QWidget *parent = 0);
  virtual ~SetupTabPrepare();

public slots:
  void matrixChanged();

private:
  Ui::SetupTabPrepare *ui;
  Preset *mPreset;
  SetupTabPreparePreprocessing *mTabPreprocessing;
  SetupTabPrepareScanning *mTabScanning;

private slots:
};

#endif // SETUPTABPREPARE_H
