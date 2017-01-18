#include "bullet.h"
#include "frameFactory.h"
Bullet::Bullet(const std::string& n, const Vector2f& pos, const Vector2f& vel):
                         Drawable(n, pos, vel),lifetime(Gamedata::getInstance().getXmlInt("bullet/lifetime")),
                         currentLife(Gamedata::getInstance().getXmlInt("bullet/currentLife")),
                         frame( FrameFactory::getInstance().getFrame(n)),
                         endY(Gamedata::getInstance().getXmlInt(n+"/loc/y"))
{
}

Bullet::Bullet(const Bullet& b): Drawable(b.getName(), b.getPosition(), b.getVelocity()),lifetime(b.lifetime),currentLife(b.currentLife),frame( FrameFactory::getInstance().getFrame("bullet")),endY(b.endY){ }

void Bullet::reset(const Vector2f &pos, const Vector2f &vel){
	setPosition(pos);
	setVelocity(vel);
	currentLife = Gamedata::getInstance().getXmlInt("bullet/currentLife");
}

void Bullet::draw() const {
	Uint32 x = static_cast<Uint32>(X());
  	Uint32 y = static_cast<Uint32>(Y());
  	frame->draw(x, y);
}

bool Bullet::isAlive() {
	if(currentLife>lifetime)
		return false;
	return true;
}

void Bullet::update(Uint32 ticks) {
	
	Vector2f incr = getVelocity()  * static_cast<float>(ticks) * 0.001;
        if(currentLife < lifetime/4 || Y()>=endY)
	{
		incr[1] = 0;
	}

  	setPosition(getPosition() + incr);	
	currentLife += abs(incr[0]);	
}


