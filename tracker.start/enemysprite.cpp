#include "enemysprite.h"
#include "explodingSprite.h"
#include <cmath>
float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

EnemySprite::EnemySprite( const std::string& name,int w, int h) : MultiSprite(name),explosion(NULL), playerPos(), attackDistance(Gamedata::getInstance().getXmlInt(getName()+"/attackDistance")), playerWidth(w),
 playerHeight(h){}


EnemySprite::EnemySprite( const EnemySprite& t) : MultiSprite(t),explosion(t.explosion),  playerPos(t.playerPos), attackDistance(t.attackDistance), playerWidth(t.playerWidth),
 playerHeight(t.playerHeight){}

EnemySprite::~EnemySprite()
{
	if(explosion)
		delete explosion;
}

void EnemySprite::reset()
{
	setPosition( Vector2f(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"),
                             Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")) );
}

void EnemySprite::explode()
{
        if(explosion)
                return;
        Sprite sprite(getName(), getPosition(),getVelocity(),frames[currentFrame]);
        explosion = new ExplodingSprite(sprite);
}

void EnemySprite::draw() const
{
	if(explosion)
	{
		explosion->draw();
	}
	else
	{
		MultiSprite::draw();		
	}
}

void EnemySprite::update(Uint32 ticks) 
{
	  advanceFrame(ticks);
	  if(explosion)
 	  {
		explosion->update(ticks);
		if(explosion->chunkCount() == 0)
		{
			delete explosion;
			explosion = NULL;
			reset();
		}
	   return;
	   
	   }
	   
	   MultiSprite::update(ticks);
	   float x= X()+getFrame()->getWidth()/2;
       float y= Y()+getFrame()->getHeight()/2;
       float ex= playerPos[0]+playerWidth/2;
       float ey= playerPos[1]+playerHeight/2;
       float distanceToEnemy = ::distance( x, y, ex, ey );
       if(distanceToEnemy > attackDistance ) {
        if( (velocityX() > 0 && playerPos[0] < X())   || (velocityX() < 0 && playerPos[0] > X() ) )
        velocityX(velocityX()*(-1));
	}
 }
void EnemySprite::advanceFrame(Uint32 ticks) {
   timeSinceLastFrame += ticks;
   Vector2f currVelocity = getVelocity();
      
   if(timeSinceLastFrame > frameInterval) {
        if(currVelocity[0] < 0 ) {
	    currentFrame = (currentFrame + 1) % (numberOfFrames/2);
	    timeSinceLastFrame = 0;
	}
	else if(currVelocity[0] >= 0 ) {
	    currentFrame = (numberOfFrames/2) + ((currentFrame +1)%(numberOfFrames/2));
	    timeSinceLastFrame = 0;
	}
   }
}
