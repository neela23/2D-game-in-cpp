#ifndef KEYSPRITE__H
#define KEYSPRITE__H
#include <string>
#include "drawable.h"

class KeySprite : public Drawable {
public:
  KeySprite(const std::string&);
  KeySprite(const KeySprite& s);
  virtual ~KeySprite() { } 
  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void reset();

private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const KeySprite*) const;
  KeySprite& operator=(const KeySprite&);
  
};
#endif
