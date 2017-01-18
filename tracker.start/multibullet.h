#include<vector>
#include<list>
#include "drawable.h"
#include "bullet.h"
#include "collisionStrategy.h"

class MultiBullet: public Drawable
{
     private:
	std::list<Bullet> bulletList;
        std::list<Bullet> freeList;
	std::string bulletName;
	Vector2f bulletSpeed;
        const unsigned int poolSize;
        std::vector<CollisionStrategy*> strategies;
     public:
	virtual const Frame* getFrame() const { return NULL; }
	MultiBullet(const std::string& name);
        virtual ~MultiBullet();
	void draw() const;
	virtual void update(Uint32 ticks);
	void shootBullet(const Vector2f &, bool isLeft);
        Uint32 getBulletListSize() const;
        Uint32 getFreeListSize() const;
        virtual bool collidedWith(const Drawable*) const;		

};
