#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ctor
Actor::Actor(int imageID, int startX, int startY, int startDirection, int depth, int size)
: GraphObject(imageID, startX, startY, startDirection, depth, size) {
    m_isAlive = true;
}

//dtor
Actor::~Actor() {} //don't know what it does yet
