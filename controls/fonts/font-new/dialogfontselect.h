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

#ifndef DIALOGFONTSELECT_H
#define DIALOGFONTSELECT_H

#include <QDialog>

namespace Ui
{
class DialogFontSelect;
}
class QModelIndex;
class QItemSelection;

namespace Data
{
namespace Containers
{
struct FontParameters;
}
}

namespace AppUI
{
namespace Fonts
{
class CharactersModel;
class DialogFontSelectData;
class UnicodeBlocksModel;
class UnicodeBlocksFilterModel;

class DialogFontSelect : public QDialog
{
  Q_OBJECT

public:
  explicit DialogFontSelect(QWidget *parent = 0);
  virtual ~DialogFontSelect();

  QString characters();
  void setCharacters(const QString &value);

  void getFontParameters(Data::Containers::FontParameters *parameters);
  void setFontParameters(const Data::Containers::FontParameters &parameters);
private:
  Ui::DialogFontSelect *ui;
  DialogFontSelectData *mData;

  void loadSettings();
  void saveSettings() const;

  void connectFontFamilyList(bool value);
  void connectFontStyleList(bool value);
  void connectFontSizeList(bool value);

private slots:
  void on_lineEdit_textChanged(const QString &value);
  void on_tableView_doubleClicked(const QModelIndex &index);
  void on_pushButtonAppendSelected_clicked();
  void on_pushButtonAppendRange_clicked();
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void on_pushButtonForeColor_clicked();
  void on_pushButtonBackColor_clicked();
  void on_spinBoxMultiplicityHeight_valueChanged(int value);
  void on_spinBoxMultiplicityWidth_valueChanged(int value);

  void on_stylesListChanged(const QStringList &list, const QString &selected);
  void on_sizesListChanged(const QList<int> &list, int selected);
  void on_charactersListChanged(const QString &value);
  void on_fontChanged(const QFont &value);
  void on_monospacedChanged(bool value);
  void on_fontMeasured(int count, int maxWidth, int maxHeight);
  void updateColorIcons(const QColor &foreground, const QColor &background);
  void on_multiplicityChanged(int height, int width);
};

} // namespace Fonts
} // namespace AppUI

#endif // DIALOGFONTSELECT_H

