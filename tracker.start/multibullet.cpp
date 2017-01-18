#include "multibullet.h"
#include "gamedata.h"
#include <list>
MultiBullet::MultiBullet(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/loc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/loc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speed/x"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/y"))
           ),bulletList(),freeList(),bulletName(name),bulletSpeed(Vector2f(Gamedata::getInstance().getXmlInt(name+"/speed/x"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/y"))),poolSize(Gamedata::getInstance().getXmlInt(name+"/bulletCount")),strategies()
{
          strategies.push_back( new RectangularCollisionStrategy );
          strategies.push_back( new PerPixelCollisionStrategy );
}




MultiBullet::~MultiBullet() {
  std::vector<CollisionStrategy*>::iterator it = strategies.begin();
  while(it!=strategies.end()){
        delete *it;
        ++it;
  }
}


void MultiBullet::draw() const{ 
	std::list<Bullet>::const_iterator it = bulletList.begin();
	while(it != bulletList.end()){
		it->draw();
		it++;
	}
}

bool MultiBullet::collidedWith(const Drawable* obj) const {
	std::list<Bullet>::const_iterator it = bulletList.begin();
        while(it != bulletList.end()){
        	if(strategies[0]->execute(*it,*obj))
       		{
                	return strategies[1]->execute(*it,*obj);
        	}
		it++;
	}
	        return false;	
}
void MultiBullet::update(Uint32 ticks){
	
	std::list<Bullet>::iterator it = bulletList.begin();
        while(it != bulletList.end()){
		if(it->isAlive()){
         	   it->update(ticks);
		   it++;
		}
		else {
		   Bullet b = *it;
		   it = bulletList.erase(it);
		   freeList.push_back(b);
		}
	}
	
}
void MultiBullet::shootBullet(const Vector2f &pos, bool isLeft){
	Vector2f bulletVel = bulletSpeed;
	Vector2f bulletPos = pos;
	if(!isLeft){
		bulletVel[0]*=-1; // Negate speed to change direction	
		bulletPos = bulletPos - Vector2f(120,0);
	}
	if(!freeList.empty()){
		Bullet b = freeList.front();
		freeList.pop_front();
		b.reset(bulletPos,bulletVel);
		bulletList.push_back(b);

	}
	else if(bulletList.size() < poolSize){
		Bullet b(bulletName,bulletPos,bulletVel);
		bulletList.push_back(b);

	}
}

Uint32 MultiBullet::getBulletListSize() const {
	  return bulletList.size();
}
Uint32 MultiBullet::getFreeListSize() const {
	  return freeList.size();	
			
}
