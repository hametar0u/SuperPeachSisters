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
    return min(m_x, x) + SPRITE_WIDTH > max(m_x, x)
        && min(m_y, y) + SPRITE_HEIGHT > max(m_y, y);
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
    m_powerups = {};
    remaining_jump_distance = 0;
    remaining_invincibility = 0;
    remaining_temporary_invincibility = 0;
    time_to_recharge_before_next_fire = 0;
}

void Peach::doSomething() {
    if (!isAlive())
        return;
    if (remaining_invincibility > 0) //instead of setting another variable to be T/F it's probably easier just to check if this variable > 0
        remaining_invincibility--;
    if (remaining_temporary_invincibility > 0)
        remaining_temporary_invincibility--;
    if (time_to_recharge_before_next_fire > 0)
        time_to_recharge_before_next_fire--;
    if (world()->objectAt(x(), y()))
        world()->bonkObjectsAt(x(), y());
    
    int target_x = x(); //TODO: this ordering might screw smt up later; come back to it
    int target_y = y();
    
    if (remaining_jump_distance > 0) {
        target_y += 4;
        if (world()->obstacleAt(target_x, target_y)) {
            world()->bonkObjectsAt(target_x, target_y);
            remaining_jump_distance = 0;
        }
        else {
            moveTo(target_x, target_y);
            setPos(target_x, target_y);
            remaining_jump_distance--;
        }
    }
    else {
        
        if (!world()->objectBelow(target_x, target_y)) {
            target_y -= 4;
            moveTo(target_x, target_y);
            setPos(target_x, target_y);
        }

    }
    
    int keyPress;
    if (world()->getKey(keyPress)) {
        switch(keyPress) {
            case KEY_PRESS_LEFT:
                setDirection(180);
                target_x -= 4;
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                target_x += 4;
                break;
            case KEY_PRESS_UP:
                if (world()->objectBelow(target_x, target_y)) {
                    if (m_powerups.find("JumpPower") != m_powerups.end()) {
                        remaining_jump_distance = 12;
                    }
                    else {
                        remaining_jump_distance = 20; //TODO: change back to 8 in prod
                    }
                    world()->playSound(SOUND_PLAYER_JUMP);
                    return;
                }
                break;
            case KEY_PRESS_SPACE:
                if (m_powerups.find("ShootPower") != m_powerups.end() && time_to_recharge_before_next_fire <= 0) {
                    world()->playSound(SOUND_PLAYER_FIRE);
                    time_to_recharge_before_next_fire = 8;
                    //TODO: fireball implementation
                }
                break;
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

void Peach::bonk() {
    m_hp--;
    if (m_hp <= 0) {
        world()->decLives();
    }
}

//================================================== OBSTACLE ==================================================//

Obstacle::Obstacle(StudentWorld* StudentWorld, int imageID, int startX, int startY) : Actor(StudentWorld, imageID, startX, startY, 0, 2 /*default size*/) {}

void Obstacle::doSomething() { return; } //it's supposed to do nothing


//================================================== BLOCK ==================================================//

Block::Block(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_BLOCK, x, y), wasBonked(false) {}

void Block::bonk() { return; } //TODO: release goodie

//================================================== PIPE ==================================================//

Pipe::Pipe(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_PIPE, x, y) {}

//================================================== FLAGS ==================================================//

Flag::Flag(StudentWorld* StudentWorld, int startX, int startY) : Actor(StudentWorld, IID_FLAG, startX, startX) {}

void Flag::doSomething() {
    if (isAlive())
        return;
    if (world()->overlapsWithPeach(x(), y())) {
        world()->increaseScore(1000);
        toggleAlive();
        world()->finishLevel();
    }
}
