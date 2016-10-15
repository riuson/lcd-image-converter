/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#ifndef IMAGEEXPORT_H
#define IMAGEEXPORT_H

#include <QObject>
#include <QStringList>
#include "ioperation.h"

namespace Operations
{

class ImageExport : public QObject, public IOperation
{
  Q_OBJECT
  Q_INTERFACES(Operations::IOperation)

public:
  explicit ImageExport(QWidget *parentWidget = 0, QObject *parent = 0);

  bool prepare(const IDocument *doc, const QStringList &keys);
  void applyDocument(IDocument *doc, const QStringList &keys);
  void applyItem(IDocument *doc, const QString &itemKey);

private:
  QWidget *mParentWidget;
  QStringList mExportFilenames;
  int mExportIndex;

  void prepareFilenames(const QStringList &keys, const QString &filename, const QString &ext);
};

}

#endif // IMAGEEXPORT_H
