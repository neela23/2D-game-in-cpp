#include "twowaysprite.h"
#include "clock.h"
#include "gamedata.h"
#include "explodingSprite.h"
#include "sprite.h"

class CollisionStrategy;

TwoWaySprite::TwoWaySprite( const std::string& name) : 
              MultiSprite(name),isJumping(0),
              idleFrames(Gamedata::getInstance().getXmlInt(getName()+"/idleframe")),
              idleTime(0),
              idleTimeLimit(Gamedata::getInstance().getXmlInt("idleTimeLimit")),
              explosion(NULL),
              multibullet(MultiBullet("bullet")),
              deltaTime(0),
              interval(500),
              explosionTime(0), 
              gameEnd(false), 
              isFacingLeft(true), 
              strategies(), 
              observers() 
     { 
	 
	  strategies.push_back( new RectangularCollisionStrategy );
  	  strategies.push_back( new PerPixelCollisionStrategy );  	   
	  setVelocity(Vector2f(0,0));
     
     }

TwoWaySprite::TwoWaySprite( const TwoWaySprite& t) : 
              MultiSprite(t),
              isJumping(t.isJumping),
              idleFrames(Gamedata::getInstance().getXmlInt(getName()+"/idleframe")),
              idleTime(0),
              idleTimeLimit(Gamedata::getInstance().getXmlInt("idleTimeLimit")),
              explosion(NULL),
              multibullet(MultiBullet("bullet")), 
              deltaTime(t.deltaTime), 
              interval(t.interval),
              explosionTime(t.explosionTime), 
              gameEnd(false), 
              isFacingLeft(t.isFacingLeft),
              strategies(t.strategies), 
              observers(t.observers) 
     { 
	 }
TwoWaySprite::~TwoWaySprite(){
	std::vector<CollisionStrategy*>::iterator itr1 = strategies.begin();
    while(itr1 != strategies.end()){
        delete *itr1;
        itr1++;       
    }
    if(explosion)
       delete explosion;
}
bool TwoWaySprite::haveBulletsCollidedWith(const Drawable *obj)
{
	if(multibullet.collidedWith(obj))
		return true;
	return false;
}
void TwoWaySprite::shoot()
{
		resetFromIdle();
		multibullet.shootBullet(getPosition()+ Vector2f(120,35), isFacingLeft);
		deltaTime = 0;
}

void TwoWaySprite::resetFromIdle()
{
    unsigned int numFrames = numberOfFrames - idleFrames;
    idleTime = 0;
	if(currentFrame >= numFrames)
	{
		//idleTime = 0;
		if(isFacingLeft)
        	                currentFrame = 0;
                	else
                        	currentFrame = numFrames/2;
	}
}
void TwoWaySprite::advanceFrame(Uint32 ticks) 
{
   timeSinceLastFrame += ticks;
   Vector2f currVelocity = getVelocity();
   unsigned int numFrames = numberOfFrames - idleFrames;

   if(currVelocity[0]==0 && currVelocity[1]==0){
	idleTime++;
	
	 if(idleTime > idleTimeLimit){
		
		//idleTime--; // To avoid overflow - idleTime does not increase to a very high value!
		if(timeSinceLastFrame > frameInterval){
		// Detect Direction
		int cushion = numFrames;
		if(!isFacingLeft){
			cushion = numFrames + idleFrames/2;
		}
		
		currentFrame = (currentFrame>=numFrames?(cushion+((currentFrame+1)%(idleFrames/2))):cushion);
		if(currentFrame == numFrames+2 || currentFrame == numFrames+3)
				idleTime -= (idleTimeLimit/5); // To let the eyes remain closed for sometime
			timeSinceLastFrame = 0;
	    }
     }
     return;
   }
 
   if(currVelocity[0] == 0 ) {
      return; 
   }
   idleTime = 0; // Reset idle time.
   if(timeSinceLastFrame > frameInterval) {
        if(currVelocity[0] >= 0 ) {
	    currentFrame = (currentFrame + 1) % (numFrames/2);
	    isFacingLeft = true;
	    timeSinceLastFrame = 0;
	    }
	else if(currVelocity[0] < 0 ) {
	    currentFrame = (numFrames/2) + ((currentFrame +1)%(numFrames/2));
	    isFacingLeft= false;
	    timeSinceLastFrame = 0;
	}
   }
}

void TwoWaySprite::draw() const {
	if(explosion){
		explosion->draw();
	}
	else{
		Uint32 x = static_cast<Uint32>(X());
		Uint32 y = static_cast<Uint32>(Y());
	  	frames[currentFrame]->draw(x, y);
		multibullet.draw();		
	}
}
void TwoWaySprite::explode() {
	if(explosion)
		return;
    resetFromIdle();
	Sprite sprite(getName(), getPosition(),getVelocity(),frames[currentFrame]);
	explosion = new ExplodingSprite(sprite);	
}

void TwoWaySprite::update(Uint32 ticks) {
 if(deltaTime< interval)
 	deltaTime+=ticks; 
 
 if(explosion){
	explosionTime+=ticks;
	explosion->update(ticks);
	if(explosion->chunkCount() == 0 && explosionTime > 3*interval){
		explosionTime = 0;
		delete explosion;
		explosion = NULL;
		// reset health here??
	}
	return;
 }
  multibullet.update(ticks); 
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < (worldHeight-frameHeight)/2) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")) {
	isJumping = false;
 
   velocityY(0.0); 
  }
  if ( X() <= 0) {
    velocityX(0.0);//abs(velocityX()));
  }
  if ( X() >= worldWidth-frameWidth ) {
    velocityX(0.0);//-abs( velocityX()) );
  }  
  std::list<EnemySprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

} 
void TwoWaySprite::left(){ 
  //advanceFrame(ticks);
  if(X() > 0)//&& X() < worldWidth-frameWidth)
  velocityX(Gamedata::getInstance().getXmlInt(getName()+"/speedX"));

}

void TwoWaySprite::right(){
    if(X() < worldWidth-frameWidth)
    velocityX(-(Gamedata::getInstance().getXmlInt(getName()+"/speedX")));
}

void TwoWaySprite::up(){
  if(!isJumping)
  {
   resetFromIdle();
  isJumping = 1;
  velocityY(-Gamedata::getInstance().getXmlInt(getName()+"/speedY"));
   }

}
void TwoWaySprite::stopX(){
  velocityX(0.0); 
}
void TwoWaySprite::stopY(){
 
}

Uint32 TwoWaySprite::BulletListSize(){
	
	return multibullet.getBulletListSize();
}

Uint32 TwoWaySprite::FreeListSize(){
	
	return multibullet.getFreeListSize();
}

bool TwoWaySprite::isGameOver(){
    return gameEnd;
}
void TwoWaySprite::reset()
{
	// Reset Monkey Position and Velocity
	setPosition(
           Vector2f(Gamedata::getInstance().getXmlInt("monkey/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt("monkey/startLoc/y")) 
           );
	setVelocity(Vector2f(0,0));
    isJumping = false;
    gameEnd = false;
}
bool TwoWaySprite::collidedWith(const Drawable* obj) 
{
	const Drawable *self = this;
	if(strategies[0]->execute(*self,*obj))
	{
	       if(obj->getName() == "key"){
         	gameEnd = strategies[1]->execute(*self,*obj);
               }
            return strategies[1]->execute(*self,*obj);
                
	}
	return false; 
}
void TwoWaySprite::detach( EnemySprite* o ) {
  std::list<EnemySprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
