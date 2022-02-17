#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//================================================== ACTOR ==================================================//

Actor::Actor(StudentWorld* StudentWorld, int imageID, int startX, int startY, int startDirection, int depth, int size)
: GraphObject(imageID, startX, startY, startDirection, depth, size) {
    m_isAlive = true;
    m_StudentWorld = StudentWorld;
    m_x = startX;
    m_y = startY;
}

bool Actor::isAt(int x, int y) {
    return x == m_x && y == m_y; //TODO: check 4 pixels around x & y
}

void Actor::setX(int new_x) {
    //TODO: bound checking?
    m_x = new_x;
}

void Actor::setY(int new_y) {
    //TODO: bound checking?
    m_y = new_y;
}

void Actor::setPos(int new_x, int new_y) {
    //TODO: bound checking
    m_x = new_x;
    m_y = new_y;
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
    
    int target_x = x();
    int target_y = y();
    
    int keyPress;
    if (world()->getKey(keyPress)) {
        if (keyPress == KEY_PRESS_LEFT) {
            setDirection(180);
            target_x -= 4;
            
        }
        else if (keyPress == KEY_PRESS_RIGHT) {
            setDirection(0);
            target_x += 4;
        }
        
        //TODO: check if object at destination position blocks movement
        if (world()->objectAt(target_x, target_y)) {
            world()->bonkObjectsAt(target_x, target_y);
            return;
        }
        else {
            moveTo(target_x, target_y);
            setPos(target_x, target_y);
        }
    }
    
    
}

//================================================== OBSTACLE ==================================================//

Obstacle::Obstacle(StudentWorld* StudentWorld, int imageID, int startX, int startY) : Actor(StudentWorld, imageID, startX, startY, 0, 2 /*default size*/) {}

void Obstacle::blockPath() { return; } //TODO: actually block path
void Obstacle::doSomething() { return; } //it's supposed to do nothing


//================================================== BLOCK ==================================================//

Block::Block(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_BLOCK, x, y), wasBonked(false) {}

void Block::getBonked() { return; } //TODO: release goodie

//================================================== PIPE ==================================================//

Pipe::Pipe(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_PIPE, x, y) {}

//================================================== FLAGS ==================================================//

Flag::Flag(StudentWorld* StudentWorld, int startX, int startY) : Actor(StudentWorld, IID_FLAG, startX, startX) {}

void Flag::doSomething() {}
