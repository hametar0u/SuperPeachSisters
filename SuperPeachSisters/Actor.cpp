#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//================================================== ACTOR ==================================================//

Actor::Actor(int imageID, int startX, int startY, int startDirection, int depth, int size)
: GraphObject(imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size) {
    m_isAlive = true;
}

//================================================== OBSTACLE ==================================================//

Obstacle::Obstacle(int imageID, int startX, int startY) : Actor(imageID, startX, startY, 0, 2 /*default size*/) {}

void Obstacle::blockPath() { return; } //TODO: actually block path


//================================================== BLOCK ==================================================//

Block::Block(int x, int y) : Obstacle(IID_BLOCK, x, y), wasBonked(false) {}
void Block::doSomething() { return; } //it's supposed to do nothing
void Block::getBonked() { return; } //TODO: release goodie
