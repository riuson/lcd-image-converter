/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#include "actionimagehandlers.h"

#include "imainwindow.h"
#include "ieditor.h"
#include "documentoperator.h"
#include "imageflip.h"
#include "imagerotate.h"
#include "imageshift.h"
#include "imageinverse.h"
#include "imageresize.h"
#include "imagegrayscale.h"
#include "imageimport.h"
#include "imageexport.h"
#include "imageeditinexternaltool.h"

ActionImageHandlers::ActionImageHandlers(QObject *parent) :
  ActionHandlersBase(parent)
{
  this->mRunningError = false;
}

void ActionImageHandlers::flipHorizontal_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageFlip imageFlip(this);
    imageFlip.setOrientation(true, false);
    docOp.apply(this->editor()->document(), imageFlip);
  }
}

void ActionImageHandlers::flipVertical_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageFlip imageFlip(this);
    imageFlip.setOrientation(false, true);
    docOp.apply(this->editor()->document(), imageFlip);
  }
}

void ActionImageHandlers::rotate_90_Clockwise_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageRotate imageRotate(this);
    imageRotate.setAngle(Operations::ImageRotate::Angle::A90);
    docOp.apply(this->editor()->document(), imageRotate);
  }
}

void ActionImageHandlers::rotate_180_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageRotate imageRotate(this);
    imageRotate.setAngle(Operations::ImageRotate::Angle::A180);
    docOp.apply(this->editor()->document(), imageRotate);
  }
}

void ActionImageHandlers::rotate_90_Counter_Clockwise_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageRotate imageRotate(this);
    imageRotate.setAngle(Operations::ImageRotate::Angle::A270);
    docOp.apply(this->editor()->document(), imageRotate);
  }
}

void ActionImageHandlers::shift_left_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageShift imageShift(this);
    imageShift.setDirection(Operations::ImageShift::Direction::Left);
    docOp.apply(this->editor()->document(), imageShift);
  }
}

void ActionImageHandlers::shift_right_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageShift imageShift(this);
    imageShift.setDirection(Operations::ImageShift::Direction::Right);
    docOp.apply(this->editor()->document(), imageShift);
  }
}

void ActionImageHandlers::shift_up_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageShift imageShift(this);
    imageShift.setDirection(Operations::ImageShift::Direction::Up);
    docOp.apply(this->editor()->document(), imageShift);
  }
}

void ActionImageHandlers::shift_down_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageShift imageShift(this);
    imageShift.setDirection(Operations::ImageShift::Direction::Down);
    docOp.apply(this->editor()->document(), imageShift);
  }
}

void ActionImageHandlers::inverse_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageInverse imageInverse(this);
    docOp.apply(this->editor()->document(), imageInverse);
  }
}

void ActionImageHandlers::resize_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageResize imageResize(this->mMainWindow->parentWidget(), this);
    docOp.apply(this->editor()->document(), imageResize);
  }
}

void ActionImageHandlers::grayscale_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageGrayscale imageGrayscale(this);
    docOp.apply(this->editor()->document(), imageGrayscale);
  }
}

void ActionImageHandlers::import_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageImport imageImport(this->mMainWindow->parentWidget(), this);
    docOp.apply(this->editor()->document(), imageImport);
  }
}

void ActionImageHandlers::export_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageExport imageExport(this->mMainWindow->parentWidget(), this);
    docOp.apply(this->editor()->document(), imageExport);
  }
}

void ActionImageHandlers::edit_in_external_tool_triggered()
{
  if (this->editor() != NULL) {
    QStringList keys = this->editor()->selectedKeys();

    Operations::DocumentOperator docOp(this);
    docOp.setKeys(keys);
    Operations::ImageEditInExternalTool imageEdit(this->mMainWindow->parentWidget(), this);
    docOp.apply(this->editor()->document(), imageEdit);
  }
}

