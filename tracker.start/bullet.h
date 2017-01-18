#include<vector>
#include<list>
#include "frame.h"
#include "drawable.h"

class Bullet: public Drawable
{	
     public:
	  virtual const Frame* getFrame() const { return frame; }
          Bullet(const std::string&, const Vector2f& pos, const Vector2f& vel);   
          Bullet(const Bullet&);
	  void draw() const;
	  virtual void update(Uint32 ticks);
	  bool isAlive();
	  void reset(const Vector2f &pos, const Vector2f &vel);
     private:
	  double lifetime;
	  double currentLife;	
	  Frame *frame;
	  const int endY;
          Bullet();
          Bullet& operator=(const Bullet&);
};
