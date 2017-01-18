#include "ioManager.h"
#include "aaline.h"
#include "clock.h"

class Hud {

public:
  Hud();
  void draw() const;
  void PoolHud(const Uint32 bulletListSize,const Uint32 freeListSize) const;
  void resetMenu() const;
  void GodMode(const int mode) const;
private:
  Uint32 width;
  Uint32 height;
  Uint32 startX;
  Uint32 startY;
  const IOManager& io;
  SDL_Surface* screen;
  Clock& clock;
  Hud(const Hud&);
  Hud& operator=(const Hud&);
 

};

