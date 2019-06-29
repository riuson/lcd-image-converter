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

#ifndef FONTDOCUMENT_H
#define FONTDOCUMENT_H

#include <QObject>
#include <QFont>

#include "idocument.h"

template <class T1, class T2> class QMap;

namespace Settings
{
namespace Presets
{
class Preset;
}
}

namespace Parsing
{
class ParsedImageData;
class TagsList;
}

namespace Data
{
namespace Containers
{

struct FontParameters;

class FontDocument : public QObject, public IDocument
{
  Q_OBJECT
  Q_INTERFACES(Data::Containers::IDocument)

public:
  explicit FontDocument(QObject *parent = 0);
  virtual ~FontDocument();

  bool load(const QString &fileName) Q_DECL_OVERRIDE;
  bool save(const QString &fileName) Q_DECL_OVERRIDE;
  bool changed() const Q_DECL_OVERRIDE;
  QString documentFilename() const Q_DECL_OVERRIDE;
  QString documentName() const Q_DECL_OVERRIDE;
  void setDocumentName(const QString &value) Q_DECL_OVERRIDE;
  QString outputFilename() const Q_DECL_OVERRIDE;
  void setOutputFilename(const QString &value) Q_DECL_OVERRIDE;
  DataContainer *dataContainer() const Q_DECL_OVERRIDE;
  QString convert(Settings::Presets::Preset *preset) Q_DECL_OVERRIDE;

  void beginChanges() Q_DECL_OVERRIDE;
  void endChanges(bool suppress) Q_DECL_OVERRIDE;
  bool canUndo() Q_DECL_OVERRIDE;
  bool canRedo() Q_DECL_OVERRIDE;
  void undo() Q_DECL_OVERRIDE;
  void redo() Q_DECL_OVERRIDE;

  void fontCharacters(QString *chars,
                      Data::Containers::FontParameters *parameters);
  void setFontCharacters(const QString &chars,
                         const Data::Containers::FontParameters &parameters);

private:
  DataContainer *mContainer;
  int mNestedChangesCounter;

  void setDocumentFilename(const QString &value);

  QFont usedFont() const;
  void setUsedFont(const QFont &value);

  QString usedStyle() const;
  void setUsedStyle(const QString &value);

  bool monospaced() const;
  void setMonospaced(const bool value);

  bool antialiasing() const;
  void setAntialiasing(const bool value);

  QColor foreground() const;
  void setForeground(const QColor value);

  QColor background() const;
  void setBackground(const QColor value);

  int ascent() const;
  void setAscent(int value);

  int descent() const;
  void setDescent(int value);

  int multiplicityWidth() const;
  void setMultiplicityWidth(const int value);

  int multiplicityHeight() const;
  void setMultiplicityHeight(const int value);

  void prepareImages(Settings::Presets::Preset *preset, const QStringList &orderedKeys, QMap<QString, Parsing::ParsedImageData *> *images, const Parsing::TagsList &tags) const;
  QString hexCode(const QString &key, const QString &encoding, bool bom) const;
  const QStringList sortKeysWithEncoding(const QStringList &keys, Settings::Presets::Preset *preset) const;

  const QString escapeUserCharacters(Settings::Presets::Preset *preset, const QString &value) const;

private slots:
  void mon_container_dataChanged(bool historyStateMoved);

signals:
  void documentChanged();
};

} // namespace Containers
} // namespace Data

#endif // FONTDOCUMENT_H
