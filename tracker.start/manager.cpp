#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <list>
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "twowaysprite.h"
#include "enemysprite.h"
#include "keysprite.h"

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  SDL_FreeSurface(screen);
  SDL_FreeSurface(bananaSurface);
  SDL_FreeSurface(pineappleSurface);  
  std::vector<Drawable*>::iterator ptr = sprites.begin();
    while(ptr!=sprites.end()) {
     delete *ptr; 
     ptr++;
   }
  std::list<ScaledSprite*>::iterator it = fruits.begin();
  while(it!=fruits.end()) {
    delete *it;
    it++;
     
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  showHud(Gamedata::getInstance().getXmlBool("hud/showHud")),
  poolHud(Gamedata::getInstance().getXmlBool("hud/poolHud")),
  screen( io.getScreen() ),
  bananaSurface (io.loadAndSet(Gamedata::getInstance().getXmlStr("banana/file"), 
              Gamedata::getInstance().getXmlBool("banana/transparency")) ),
  pineappleSurface(io.loadAndSet(Gamedata::getInstance().getXmlStr("pineapple/file"), 
              Gamedata::getInstance().getXmlBool("pineapple/transparency")) ),            
  night_sky("night_sky", Gamedata::getInstance().getXmlInt("night_sky/factor") ),
  front_building("front_building", Gamedata::getInstance().getXmlInt("front_building/factor") ),
  platform("platform", Gamedata::getInstance().getXmlInt("platform/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  hud(),
  health(),
  fruits(),
  worldwidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldheight(Gamedata::getInstance().getXmlInt("world/height")),
  showResetHud(false),
  mode(NORMAL),
  sound()
  {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  makeFruits();
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.reserve(2);
  sprites.push_back( new TwoWaySprite("monkey") );
  sprites.push_back( new EnemySprite("crocodile", sprites[0]->getFrame()->getWidth()/2, 
                        sprites[0]->getFrame()->getHeight()/2));
  static_cast<TwoWaySprite*>(sprites[0])->attach(static_cast<EnemySprite*>(sprites[1]));
  sprites.push_back( new KeySprite("key"));
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::makeFruits() {
  unsigned numberOfFruits = Gamedata::getInstance().getXmlInt("numberOfFruits");
  for (unsigned i = 0; i < numberOfFruits; i+=2) 
  {
    fruits.push_back( new ScaledSprite("pineapple", pineappleSurface));
  }
  fruits.sort(ScaledSpriteCompare());
}

void Manager::drawFruits(std::list<ScaledSprite*>::const_iterator &it, float reqScale) const {
	
  while(it!= fruits.end() && (*it)->getScale() == reqScale)
  {
	(*it)->draw();
	it++;
  }
	
}

void Manager::draw() const {
  std::list<ScaledSprite*>::const_iterator start_index = fruits.begin();
  night_sky.draw();
  drawFruits(start_index,0.5);
  front_building.draw();
  drawFruits(start_index,1);
  platform.draw();
  drawFruits(start_index,1.5);
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  io.printMessageAt(title, 10, 450);
  if(showHud) {
    hud.draw();
  }
  if(poolHud){
    hud.PoolHud(static_cast<TwoWaySprite*>(sprites[0])->BulletListSize(),static_cast<TwoWaySprite*>(sprites[0])->FreeListSize());
  }
  health.draw();
  if(static_cast<TwoWaySprite*>(sprites[0])->isGameOver() && showResetHud){
  hud.resetMenu();
  }
  if(mode == NORMAL)
  hud.GodMode(NORMAL);
  else
  hud.GodMode(GOD);
  viewport.draw();
  SDL_Flip(screen);
 
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::videoMake()
{
	if ( makeVideo && frameCount < frameMax ) 
	{
	    makeFrame();
  	}
}
void Manager::update() {  
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();
  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
  }
  videoMake();
  for (unsigned int i = 0; i < sprites.size(); ++i) {
	  sprites[i]->update(ticks);
  }
    if(sprites[0]->collidedWith(sprites[2])) {
		sound[3];
		sprites[2]->setPosition(Vector2f(worldwidth+200,0));
		sound.toggleMusic();
    }
     // Check if bullets have collided!
	if(static_cast<TwoWaySprite*>(sprites[0])->haveBulletsCollidedWith(sprites[1]))
	{
		if(!sprites[1]->isExploding() )
		   sound[2];
		sprites[1]->explode();
	}
   
		if(sprites[0]->collidedWith(sprites[1])){  
			 if( mode == NORMAL){
				if(!sprites[0]->isExploding() )
			       sound[2];
			    sprites[0]->explode();
				health.reset();	
		    }
		    else{
			   if(!sprites[1]->isExploding() )
			      sound[2];
				sprites[1]->explode();
			} 
		}     
   
 /* if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }*/ 
  night_sky.update();
  front_building.update();
  platform.update();
  std::list<ScaledSprite*>::iterator ptr = fruits.begin();
  while(ptr != fruits.end()) 
  {
    (*ptr)->update(ticks);
    
    if(mode == NORMAL){
       bool isCollided = (*ptr)->collidedWith(sprites[0]);
       if(isCollided)
       { 
		
	     health.update(ticks);
	     if(health.isHealthZero()){
			 if(!sprites[0]->isExploding() )
		        sound[2];
           sprites[0]->explode();
           health.reset();
         }
	     ScaledSprite *erasedBanana = *ptr;
	     erasedBanana->setPosition(Vector2f(rand()% worldwidth,
                    -rand()% worldheight));
       }
    }
    ptr++;
  }
  viewport.update(); // always update viewport last
}

void Manager::reset(){
   sound.toggleMusic();
   sprites[0]->reset();	  
   sprites[1]->reset(); 
   sprites[2]->reset();                                                      
   health.reset();
   mode = NORMAL;
}
void Manager::play() {
  SDL_Event event;
 
  bool done = false;
  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
	      sprites[0]->explode();
        }
        if( keystate[SDLK_a] ) {
         static_cast<TwoWaySprite*>(sprites[0])->left();
        }
        if( keystate[SDLK_d] ) {
           static_cast<TwoWaySprite*>(sprites[0])->right();
        }
        if( keystate[SDLK_w] ) {
           static_cast<TwoWaySprite*>(sprites[0])->up();
        }
	    if( keystate[SDLK_s] && !showResetHud ){
		   sound[0];
		   sprites[0]->shoot();
	    }
	     if( keystate[SDLK_g] && !showResetHud ){
		  if(mode == GOD)  {
			 mode = NORMAL;
			 hud.GodMode(NORMAL);
		  }
		  else {
			  mode = GOD;
			  hud.GodMode(GOD);
		  }
	    }
        if(showResetHud && keystate[SDLK_r] ) {
          reset();
          showResetHud=false;
        
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if(keystate[SDLK_F1]) {
         showHud = !showHud;
        }
         if(keystate[SDLK_F2]) {
         poolHud = !poolHud;
        }
      }
      if ( event.type == SDL_KEYUP) {
        if( !keystate[SDLK_w] ) {
           static_cast<TwoWaySprite*>(sprites[0])->stopY();
	    }
        if( !keystate[SDLK_a] && !keystate[SDLK_d] ) {
           static_cast<TwoWaySprite*>(sprites[0])->stopX();
	    }   
      }  
    }
    if(clock.getSeconds() == 2){
    showHud = 0;
    }
    draw();
    if(!static_cast<TwoWaySprite*>(sprites[0])->isGameOver()) //Freeze the game when game is over (When monkey takes the key) 
      update();
    else {
      showResetHud = true;
      videoMake(); 
      }
  }
}
