/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2022 riuson
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

#include "imagealign.h"

#include "alignmodes.h"
#include "alignmodinfo.h"
#include "bitmaphelper.h"
#include "datacontainer.h"
#include "dialogalign.h"
#include "idocument.h"

namespace Operations
{

ImageAlign::ImageAlign(QWidget* parentWidget, QObject* parent) : QObject(parent) { this->mParentWidget = parentWidget; }

bool ImageAlign::prepare(const Data::Containers::IDocument* doc, const QStringList& keys)
{
  AppUI::CommonDialogs::DialogAlign dialog(doc->dataContainer(), keys, this->mParentWidget);

  if (dialog.exec() == QDialog::Accepted) {
    const Data::AlignModInfo* modInfo = dialog.alignInfo();
    this->mMods = modInfo->summary();
    return true;
  }

  return false;
}

void ImageAlign::applyDocument(Data::Containers::IDocument* doc, const QStringList& keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)
}

void ImageAlign::applyItem(Data::Containers::IDocument* doc, const QString& itemKey)
{
  if (this->mMods.horizontalMode != Data::HorizontalAlignMode::None ||
      this->mMods.verticalMode != Data::VerticalAlignMode::None) {
    Data::AlignModInfo::Mods mods = this->mMods;
    const QImage* original = doc->dataContainer()->image(itemKey);
    QImage result = Parsing::Conversion::BitmapHelper::align(original, mods.horizontalMode, mods.horizontalOffset,
                                                             mods.verticalMode, mods.verticalOffset);
    doc->dataContainer()->setImage(itemKey, &result);
  }
}

} // namespace Operations
