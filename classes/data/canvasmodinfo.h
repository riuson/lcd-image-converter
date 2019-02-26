#ifndef CANVASMODINFO_H
#define CANVASMODINFO_H

class QString;

#include <QString>

namespace Data
{

class CanvasModInfo
{
public:
  struct Mods {
    qint16 left;
    qint16 top;
    qint16 right;
    qint16 bottom;

    void reset();
  };

  CanvasModInfo();

  const Mods summary() const;

  void modify(qint16 left, qint16 top, qint16 right, qint16 bottom);
  void commit();

  const QString toString() const;

private:
  Mods mCommited;
  Mods mModified;
};

} // namespace Data

#endif // CANVASMODINFO_H
