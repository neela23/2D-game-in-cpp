#include "multisprite.h"
#include "multibullet.h"
#include "collisionStrategy.h"
#include "enemysprite.h"
class ExplodingSprite;
class TwoWaySprite : public MultiSprite {

public :
      TwoWaySprite(const std::string&);
      TwoWaySprite(const TwoWaySprite&);
      ~TwoWaySprite();
      void left();
      void right();
      void up();
      void stopX();
      void stopY();
      bool isExploding(){ if(explosion) return true; return false;}
      Uint32 BulletListSize();
      Uint32 FreeListSize();
      virtual void shoot();
      bool isGameOver();
      void reset();
      virtual bool collidedWith(const Drawable*);
      bool haveBulletsCollidedWith(const Drawable*);
      void attach( EnemySprite* o ) { observers.push_back(o); } 
      void detach( EnemySprite* o );

private:
      TwoWaySprite& operator=(const TwoWaySprite&);
      virtual void advanceFrame(Uint32 ticks);
      virtual void update(Uint32 ticks);
      virtual void explode();
      virtual void draw() const;
      int isJumping;
      unsigned int idleFrames;
      unsigned int idleTime;
      const unsigned int idleTimeLimit; 	
      ExplodingSprite *explosion;
      MultiBullet multibullet;
      int deltaTime, interval,explosionTime;
      bool gameEnd;
      bool isFacingLeft;  
      std::vector<CollisionStrategy*> strategies;
      std::list<EnemySprite*> observers;
      void resetFromIdle();
};

