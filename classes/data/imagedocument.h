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

#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include <QObject>
#include "idocument.h"

template <class T1, class T2> class QMap;
class Preset;
class ParsedImageData;
class Tags;

class ImageDocument : public QObject, public IDocument
{
  Q_OBJECT
  Q_INTERFACES(IDocument)

public:
  explicit ImageDocument(QObject *parent = 0);
  virtual ~ImageDocument();

  bool load(const QString &fileName) Q_DECL_OVERRIDE;
  bool save(const QString &fileName) Q_DECL_OVERRIDE;
  bool changed() const Q_DECL_OVERRIDE;
  QString documentFilename() const Q_DECL_OVERRIDE;
  QString documentName() const Q_DECL_OVERRIDE;
  void setDocumentName(const QString &value) Q_DECL_OVERRIDE;
  QString outputFilename() const Q_DECL_OVERRIDE;
  void setOutputFilename(const QString &value) Q_DECL_OVERRIDE;
  DataContainer *dataContainer() const Q_DECL_OVERRIDE;
  QString convert(Preset *preset) Q_DECL_OVERRIDE;

  void beginChanges() Q_DECL_OVERRIDE;
  void endChanges(bool suppress) Q_DECL_OVERRIDE;
  bool canUndo() Q_DECL_OVERRIDE;
  bool canRedo() Q_DECL_OVERRIDE;
  void undo() Q_DECL_OVERRIDE;
  void redo() Q_DECL_OVERRIDE;

private:
  DataContainer *mContainer;
  int mNestedChangesCounter;

  static const QString DefaultKey;

  void setDocumentFilename(const QString &value);

  void prepareImages(Preset *preset, QMap<QString, ParsedImageData *> *images, const Tags &tags) const;

private slots:
  void mon_container_dataChanged(bool historyStateMoved);

signals:
  void documentChanged();
};

#endif // IMAGEDOCUMENT_H
