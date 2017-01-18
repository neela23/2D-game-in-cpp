#ifndef MANAGER__H
#define MANAGER__H

#include <vector>
#include <list>
#include <SDL.h>
#include "ioManager.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "health.h"
#include "scaledSprite.h"
#include "sound.h"
enum GameMode{
	NORMAL,
	GOD
};
class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  bool showHud;
  bool poolHud;
  SDL_Surface * const screen;
  SDL_Surface * const bananaSurface;
  SDL_Surface * const pineappleSurface;
  World night_sky;
  World front_building;
  World platform;
  Viewport& viewport;
  
  std::vector<Drawable*> sprites;
  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
 
  Hud hud;
  Health health;
  std::list<ScaledSprite*> fruits;
  const Uint32 worldwidth;
  const Uint32 worldheight;
  bool showResetHud;
  GameMode mode; 
  SDLSound sound;
  void makeFruits();
  void drawFruits(std::list<ScaledSprite*>::const_iterator &it, float reqScale) const;
  void draw() const;
  void update();
  void reset();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
  void videoMake();
};
#endif
