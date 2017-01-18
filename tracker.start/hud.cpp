#include "hud.h"

Hud::Hud():
	width(Gamedata::getInstance().getXmlInt("hud/hudWidth")),
        height(Gamedata::getInstance().getXmlInt("hud/hudHeight")),
        startX(Gamedata::getInstance().getXmlInt("hud/hudStartX")),
        startY(Gamedata::getInstance().getXmlInt("hud/hudStartY")),
        io(IOManager::getInstance()), 
        screen(io.getScreen()),
        clock(Clock::getInstance())
{}

void Hud::draw() const {
//transparent background
Draw_AALine(screen,startX,startY+height/2,startX+width,startY+height/2,height/40,0,0,0,0x50);
//top border
Draw_AALine(screen,startX,startY,startX+width,startY,3.0,255,0,0,0xFF);
//bottom border
Draw_AALine(screen,startX,startY+height/2,startX+width,startY+height/2,3.0,255,0,0,0xFF);
io.printMessageValueAt("Fps:",clock.getAverageFrameRate(),startX+10,startY+30 );
io.printMessageValueAt("Sec:",clock.getSeconds(),startX+10,startY+50);
io.printMessageAt("Left: Key A",startX+10,startY+70);
io.printMessageAt("Right: Key D",startX+10,startY+90);
io.printMessageAt("Jump : Key W",startX+10,startY+110);
io.printMessageAt("Shoot : Key S",startX+10,startY+130);
io.printMessageAt("God Mode : Key G",startX+10,startY+150);
}

void Hud::GodMode(const int mode) const{

if( mode == 0)
io.printMessageAt("GOD MODE: OFF", startX+700,startY+50);
else
io.printMessageAt("GOD MODE: ON", startX+700,startY+50);
}

void Hud::PoolHud(const Uint32 bulletListSize,const Uint32 freeListSize) const {
	io.printMessageValueAt("BulletList:",bulletListSize,startX+700,startY+70);
io.printMessageValueAt("FreeList:",freeListSize,startX+700,startY+90);
}
void Hud::resetMenu() const {
 //transparent background
 Draw_AALine(screen,startX+200,startY+height/2,startX+700,startY+height/2,height,0,0,0,0x50);
 io.printMessageAt("GAME OVER", startX+370,startY+150);    
 io.printMessageAt("PRESS 'r' TO RESET", startX+370, startY+200);
}
