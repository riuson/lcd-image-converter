/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#ifndef REORDERINGOPTIONS_H
#define REORDERINGOPTIONS_H

#include <QObject>

#include "ipresetsoptionspart.h"

template <class T> class QVector;

namespace Settings
{
namespace Presets
{

class ReorderingOptions : public QObject, public IPresetOptionsPart
{
  Q_OBJECT
  Q_INTERFACES(Settings::Presets::IPresetOptionsPart)

public:
  explicit ReorderingOptions(QObject* parent = 0);
  virtual ~ReorderingOptions();

  int operationsCount() const;
  void operation(int index, quint32* mask, int* shift, bool* left) const;
  void operationAdd(quint32 mask, int shift, bool left);
  void operationRemove(int index);
  void operationsRemoveAll();
  void operationReplace(int index, quint32 mask, int shift, bool left);

  bool load(QSettings* settings) Q_DECL_OVERRIDE;
  bool loadXmlElement(QDomElement element) Q_DECL_OVERRIDE;
  void save(QSettings* settings) Q_DECL_OVERRIDE;
  void saveXmlElement(QDomElement element) Q_DECL_OVERRIDE;

  QString groupName() const Q_DECL_OVERRIDE;

private:
  static const QString GroupName;
  static const QString FieldOperations;
  static const QString FieldOperation;
  static const QString FieldMask;
  static const QString FieldShift;
  static const QString FieldLeft;

  QVector<quint32>* mOperations;

signals:
  void changed();

public slots:
};

} // namespace Presets
} // namespace Settings

#endif // REORDERINGOPTIONS_H
