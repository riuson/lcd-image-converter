/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#ifndef EDITORTABFONT_H
#define EDITORTABFONT_H

#include <QWidget>

#include "ieditor.h"

namespace Ui
{
class EditorTabFont;
}

class DataContainer;
class QSplitter;
class ImagesModel;
class ImagesScaledProxy;
class QItemSelection;
class QModelIndex;
class StatusData;
class FontDocument;
class IDocument;
struct tFontParameters;

namespace ImageEditor
{
class Editor;
}

class EditorTabFont : public QWidget, public IEditor
{
  Q_OBJECT
  Q_INTERFACES(IEditor)

public:
  explicit EditorTabFont(QWidget *parent = 0);
  virtual ~EditorTabFont();

  IDocument *document() const Q_DECL_OVERRIDE;
  QStringList selectedKeys() const Q_DECL_OVERRIDE;
  StatusData *statusData() const Q_DECL_OVERRIDE;
  EditorType type() const Q_DECL_OVERRIDE;

  void setFontCharacters(const QString &chars,
                         const tFontParameters &parameters);
  void fontCharacters(QString *chars,
                      tFontParameters *parameters);

protected:
  void changeEvent(QEvent *e) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
  Ui::EditorTabFont *ui;
  QWidget *mEditorWidget;
  ImageEditor::Editor *mEditorObject;
  FontDocument *mDocument;
  QSplitter *mSplitter;
  ImagesModel *mModel;
  ImagesScaledProxy *mScaledProxy;
  StatusData *mStatusData;
  int mLastImagesCount;

  QFont mTableFont;

  void initStatusData();
  void updateStatus();

  QString currentKey() const;
  void updateSelectedImage();

private slots:
  void mon_documentChanged();
  void mon_documentChangedSignificantly();
  void mon_editor_imageChanged(const QImage *value);
  void mon_editor_mouseMove(const QPoint *point);
  void mon_editor_scaleChanged(int scale);
  void currentChanged(const QModelIndex &current, const QModelIndex &previous);
  void updateTableFont();
  void resizeToContents();
signals:
  void documentChanged();
  void statusChanged();
};

#endif // EDITORTABFONT_H
