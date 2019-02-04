#include "canvasmodinfo.h"

namespace Data
{

CanvasModInfo::CanvasModInfo()
{
  this->mSourceSize = QSize();
  this->mChangeLeft = 0;
  this->mChangeTop = 0;
  this->mChangeRight = 0;
  this->mChangeBottom = 0;
  this->mOffsetX = 0;
  this->mOffsetY = 0;
}

const QSize &CanvasModInfo::sourceSize() const
{
  return this->mSourceSize;
}

void CanvasModInfo::setSourceSize(const QSize &size)
{
  this->mSourceSize = size;
}

void CanvasModInfo::resizeData(qint16 &changeLeft, qint16 &changeTop, qint16 &changeRight, qint16 &changeBottom) const
{
  changeLeft = this->mChangeLeft;
  changeTop = this->mChangeTop;
  changeRight = this->mChangeRight;
  changeBottom = this->mChangeBottom;
}

void CanvasModInfo::setResizeData(qint16 changeLeft, qint16 changeTop, qint16 changeRight, qint16 changeBottom)
{
  this->mChangeLeft = changeLeft;
  this->mChangeTop = changeTop;
  this->mChangeRight = changeRight;
  this->mChangeBottom = changeBottom;
}

void CanvasModInfo::offsetData(qint16 &offsetX, qint16 &offsetY) const
{
  offsetX = this->mOffsetX;
  offsetY = this->mOffsetY;
}

void CanvasModInfo::setOffsetData(qint16 offsetX, qint16 offsetY)
{
  this->mOffsetX = offsetX;
  this->mOffsetY = offsetY;
}

} // namespace Data
