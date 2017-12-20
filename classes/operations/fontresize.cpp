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

#include "fontresize.h"
#include "idocument.h"
#include "datacontainer.h"

namespace Operations
{

FontResize::FontResize(QWidget *parentWidget, QObject *parent)
  : ImageResize(parentWidget, parent)
{
}

void FontResize::applyDocument(Data::Containers::IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(keys)

  bool ok;
  int ascent = doc->dataContainer()->commonInfo("ascent").toInt(&ok);

  if (ok) {
    int descent = doc->dataContainer()->commonInfo("descent").toInt(&ok);

    if (ok) {
      ascent += this->mTop;
      descent += this->mBottom;

      doc->dataContainer()->setCommonInfo("ascent", ascent);
      doc->dataContainer()->setCommonInfo("descent", descent);
    }
  }
}

} // namespace Operations
