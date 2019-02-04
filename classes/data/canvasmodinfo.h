#ifndef CANVASMODINFO_H
#define CANVASMODINFO_H

#include <QSize>

namespace Data
{

class CanvasModInfo
{
public:
  CanvasModInfo();

  const QSize &sourceSize() const;
  void setSourceSize(const QSize &size);

  void resizeData(qint16 &changeLeft, qint16 &changeTop, qint16 &changeRight, qint16 &changeBottom) const;
  void setResizeData(qint16 changeLeft, qint16 changeTop, qint16 changeRight, qint16 changeBottom);

  void offsetData(qint16 &offsetX, qint16 &offsetY) const;
  void setOffsetData(qint16 offsetX, qint16 offsetY);

private:
  QSize mSourceSize;

  qint16 mChangeLeft;
  qint16 mChangeTop;
  qint16 mChangeRight;
  qint16 mChangeBottom;

  qint16 mOffsetX;
  qint16 mOffsetY;
};

} // namespace Data

#endif // CANVASMODINFO_H
