#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//================================================== ACTOR ==================================================//

Actor::Actor(StudentWorld* StudentWorld, int imageID, int startX, int startY, int startDirection, int depth, int size)
: GraphObject(imageID, startX, startY, startDirection, depth, size) {
    m_isAlive = true;
    m_StudentWorld = StudentWorld;
}

//================================================== PEACH ==================================================//

Peach::Peach(StudentWorld* StudentWorld, int x, int y) : Actor(StudentWorld, IID_PEACH, x, y) {
    m_hp = 1;
    m_invincible = false;
    m_powerups = {};
}

void Peach::doSomething() {
    if (m_hp <= 0)
        return;
    
    int left = KEY_PRESS_LEFT; //copium solution; any other way??
    int right = KEY_PRESS_RIGHT;
    
    if (world()->getKey(left)) {
        setDirection(180);
        //TODO: calculate target x,y position -> where is position stored anyways?
        //TODO: check if object at destination position blocks movement
            //if true, bonk() and return
            //else update location to destination
    }
    else if (world()->getKey(right)) {
        setDirection(0);
    }
} //TODO: implement directional movement

//================================================== OBSTACLE ==================================================//

Obstacle::Obstacle(StudentWorld* StudentWorld, int imageID, int startX, int startY) : Actor(StudentWorld, imageID, startX, startY, 0, 2 /*default size*/) {}

void Obstacle::blockPath() { return; } //TODO: actually block path
void Obstacle::doSomething() { return; } //it's supposed to do nothing


//================================================== BLOCK ==================================================//

Block::Block(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_BLOCK, x, y), wasBonked(false) {}

void Block::getBonked() { return; } //TODO: release goodie

//================================================== PIPE ==================================================//

Pipe::Pipe(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_PIPE, x, y) {}
