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

#ifndef IMAGEEDITINEXTERNALTOOL_H
#define IMAGEEDITINEXTERNALTOOL_H

#include <QObject>
#include <QProcess>
#include "ioperation.h"

namespace Operations
{

class ImageEditInExternalTool : public QObject, public IOperation
{
  Q_OBJECT
  Q_INTERFACES(Operations::IOperation)

public:
  explicit ImageEditInExternalTool(QWidget *parentWidget = 0, QObject *parent = 0);

  bool prepare(const IDocument *doc, const QStringList &keys) Q_DECL_OVERRIDE;
  void applyDocument(IDocument *doc, const QStringList &keys) Q_DECL_OVERRIDE;
  void applyItem(IDocument *doc, const QString &itemKey) Q_DECL_OVERRIDE;

private:
  QWidget *mParentWidget;
  bool mRunningError;

private slots:
  void processError(QProcess::ProcessError error);
};

}

#endif // IMAGEEDITINEXTERNALTOOL_H
