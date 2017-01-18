#ifndef ENEMYSPRITE__H
#define ENEMYSPRITE__H

#include "multisprite.h"
#include "drawable.h"
class ExplodingSprite;

class EnemySprite : public MultiSprite {

public :
	
      EnemySprite(const std::string&, int, int);
      EnemySprite(const EnemySprite&);
      ~EnemySprite(); 
      void setPlayerPos(const Vector2f& p) { playerPos = p; }
      bool isExploding(){ if(explosion) return true; return false;}

private:
      EnemySprite& operator=(const EnemySprite&);
      virtual void advanceFrame(Uint32 ticks);
      virtual void explode();
      virtual void update(Uint32 ticks);
      virtual void draw() const;  
      virtual void reset(); 
      ExplodingSprite *explosion;
      Vector2f playerPos;
      const float attackDistance;
      int playerWidth;
      int playerHeight;
};

#endif
