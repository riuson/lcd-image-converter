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

#include "imageresize.h"

#include "bitmaphelper.h"
#include "canvasmodinfo.h"
#include "datacontainer.h"
#include "dialogcanvasresize.h"
#include "idocument.h"

namespace Operations
{

ImageResize::ImageResize(QWidget* parentWidget, QObject* parent) : QObject(parent)
{
  this->mParentWidget = parentWidget;
}

bool ImageResize::prepare(const Data::Containers::IDocument* doc, const QStringList& keys)
{
  AppUI::CommonDialogs::DialogCanvasResize dialog(doc->dataContainer(), keys, this->mParentWidget);

  if (dialog.exec() == QDialog::Accepted) {
    const QMap<QString, Data::CanvasModInfo*>* map = dialog.resizeInfo();

    for (auto key : keys) {
      Data::CanvasModInfo* info = map->value(key);

      if (info != nullptr) {
        Data::CanvasModInfo::Mods mods = info->summary();
        this->mMap.insert(key, mods);
      }
    }

    return true;
  }

  return false;
}

void ImageResize::applyDocument(Data::Containers::IDocument* doc, const QStringList& keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)
}

void ImageResize::applyItem(Data::Containers::IDocument* doc, const QString& itemKey)
{
  if (this->mMap.contains(itemKey)) {
    Data::CanvasModInfo::Mods mods = this->mMap.value(itemKey);
    const QImage* original = doc->dataContainer()->image(itemKey);
    QImage result =
        Parsing::Conversion::BitmapHelper::crop(original, mods.left, mods.top, mods.right, mods.bottom,
                                                Parsing::Conversion::BitmapHelper::detectBackgroundColor(original));
    doc->dataContainer()->setImage(itemKey, &result);
  }
}

} // namespace Operations
