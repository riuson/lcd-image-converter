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
    int left;
    int top;
    int right;
    int bottom;

    void reset();
  };

  CanvasModInfo();

  const Mods summary() const;

  void modify(int left, int top, int right, int bottom);
  void commit();
  void reset();

  const QString toString() const;

private:
  Mods mCommited;
  Mods mModified;
};

} // namespace Data

#endif // CANVASMODINFO_H
