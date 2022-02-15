#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//================================================== ACTOR ==================================================//

Actor::Actor(int imageID, int startX, int startY, int startDirection, int depth, int size)
: GraphObject(imageID, startX, startY, startDirection, depth, size) {
    m_isAlive = true;
}

Actor::~Actor() {} //don't know what it does yet

//================================================== OBSTACLE ==================================================//

Obstacle::Obstacle(int imageID) : Actor(imageID, 0 /* TODO: x from level */, 0 /* TODO: y from level */, 0, 2 /*default size*/) {}

void Obstacle::blockPath() { return; } //TODO: actually block path

Obstacle::~Obstacle() {} //don't know what this does yet

//================================================== BLOCK ==================================================//

Block::Block() : Obstacle(IID_BLOCK), wasBonked(false) {}
void Block::doSomething() { return; } //it's supposed to do nothing
void Block::getBonked() { return; } //TODO: release goodie
