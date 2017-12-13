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

class QSplitter;
class QItemSelection;
class QModelIndex;
struct tFontParameters;

namespace Data
{
namespace Containers
{
class DataContainer;
class FontDocument;
class IDocument;
}
namespace Models
{
class ImagesModel;
class ImagesScaledProxy;
}
}

namespace AppUI
{
namespace Status
{
class StatusData;
}
}

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

  Data::Containers::IDocument *document() const Q_DECL_OVERRIDE;
  QStringList selectedKeys() const Q_DECL_OVERRIDE;
  AppUI::Status::StatusData *statusData() const Q_DECL_OVERRIDE;
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
  Data::Containers::FontDocument *mDocument;
  QSplitter *mSplitter;
  Data::Models::ImagesModel *mModel;
  Data::Models::ImagesScaledProxy *mScaledProxy;
  AppUI::Status::StatusData *mStatusData;
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
