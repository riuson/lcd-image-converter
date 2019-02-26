#include "canvasmodinfo.h"

namespace Data
{

void CanvasModInfo::Mods::reset()
{
  this->left = this->top = this->right = this->bottom = 0;
}

CanvasModInfo::CanvasModInfo()
{
  this->mCommited.reset();
  this->mModified.reset();
}

const CanvasModInfo::Mods CanvasModInfo::summary() const
{
  CanvasModInfo::Mods result;
  result.top = this->mCommited.top + this->mModified.top;
  result.left = this->mCommited.left + this->mModified.left;
  result.right = this->mCommited.right + this->mModified.right;
  result.bottom = this->mCommited.bottom + this->mModified.bottom;
  return result;
}

void CanvasModInfo::modify(qint16 left, qint16 top, qint16 right, qint16 bottom)
{
  this->mModified.left = left;
  this->mModified.top = top;
  this->mModified.right = right;
  this->mModified.bottom = bottom;
}

void CanvasModInfo::commit()
{
  this->mCommited.left += this->mModified.left;
  this->mCommited.top += this->mModified.top;
  this->mCommited.right += this->mModified.right;
  this->mCommited.bottom += this->mModified.bottom;
  this->mModified.reset();
}

const QString CanvasModInfo::toString() const
{
  return QString("L:%1,T:%2,R:%3,B:%4")
         .arg(this->mCommited.left + this->mModified.left)
         .arg(this->mCommited.top + this->mModified.top)
         .arg(this->mCommited.right + this->mModified.right)
         .arg(this->mCommited.bottom + this->mModified.bottom);
}

} // namespace Data
