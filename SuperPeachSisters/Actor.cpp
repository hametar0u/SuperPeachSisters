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

bool Actor::isAt(int x, int y) const {
    return min(m_x, x) + SPRITE_WIDTH > max(m_x, x)
        && min(m_y, y) + SPRITE_HEIGHT > max(m_y, y);
}

void Actor::setPos(int new_x, int new_y) {
    //TODO: bound checking
    moveTo(new_x, new_y);
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
            setPos(target_x, target_y);
            remaining_jump_distance--;
        }
    }
    else {
        
        if (!world()->obstacleBelow(target_x, target_y)) {
            target_y -= 4;
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
                if (world()->obstacleBelow(target_x, target_y)) {
                    if (m_powerups.find("JumpPower") != m_powerups.end()) {
                        remaining_jump_distance = 12;
                    }
                    else {
                        remaining_jump_distance = 64; //TODO: change back to 8
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
        
        if (world()->obstacleAt(target_x, target_y)) {
            world()->bonkObjectsAt(target_x, target_y);
            return;
        }
        else {
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

bool Peach::hasBuff(string buff) {
    for (string powerup : m_powerups) {
        if (powerup == buff)
            return true;
    }
    return false;
}

//================================================== OBSTACLE ==================================================//

Obstacle::Obstacle(StudentWorld* StudentWorld, int imageID, int startX, int startY) : Actor(StudentWorld, imageID, startX, startY, 0, 2 /*default size*/) {}

void Obstacle::doSomething() { return; } //it's supposed to do nothing


//================================================== BLOCK ==================================================//

Block::Block(StudentWorld* StudentWorld, int x, int y, GoodieType goodie) : Obstacle(StudentWorld, IID_BLOCK, x, y) {
    m_goodie = goodie;
}

void Block::bonk() {
    if (hasGoodie()) {
        world()->playSound(SOUND_POWERUP_APPEARS);
        releaseGoodie();
    }
    else {
        world()->playSound(SOUND_PLAYER_BONK);
    }

}
void Block::releaseGoodie() {
    int target_x = x();
    int target_y = y() + 8;
    switch (m_goodie) {
        case flower:
            world()->createActor(IID_FLOWER, target_x, target_y);
            break;
        case mushroom:
            world()->createActor(IID_MUSHROOM, target_x, target_y);
            break;
        case star:
            world()->createActor(IID_STAR, target_x, target_y);
            break;
        case none:
            break;
    }
    
    m_goodie = none;
}

//================================================== PIPE ==================================================//

Pipe::Pipe(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_PIPE, x, y) {}

//================================================== FLAGS ==================================================//

Flag::Flag(StudentWorld* StudentWorld, int startX, int startY, int imageID) : Actor(StudentWorld, imageID, startX, startY) {}

void Flag::doSomething() {
//    if (isAlive()) //TODO: implement later? its in the spec but doesn't do much
//        return;
    if (world()->overlapsWithPeach(x(), y())) {
        world()->increaseScore(1000);
        toggleAlive();
        progressNext();
    }
}

void Flag::progressNext() {
    world()->finishLevel();
}

//================================================== MARIO ==================================================//

Mario::Mario(StudentWorld* StudentWorld, int x, int y) : Flag(StudentWorld, x, y, IID_MARIO) {}

void Mario::progressNext() {
    world()->endGame();
}

//================================================== GOODIES ==================================================//

Goodie::Goodie(StudentWorld* StudentWorld, int imageID, GoodieType goodie, int x, int y) : Actor(StudentWorld, imageID, x, y, 0, 1) {
    m_goodie = goodie;
}

void Goodie::doSomething() {
    if (world()->overlapsWithPeach(x(), y())) {
        string buff;
        int points;
        switch (m_goodie) {
            case flower:
                buff = "ShootPower";
                points = 50;
                break;
            case mushroom:
                buff = "JumpPower";
                points = 75;
                break;
            case star:
                buff = "StarPower";
                points = 100;
                break;
            default: //failsafe
                buff = "";
                points = 0;
                break;
        }
        
        world()->increaseScore(points);
        world()->buffPeach(buff);
        toggleAlive();
        world()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else {
        int target_x = x();
        int target_y = y() - 2;
        if (!world()->obstacleAt(target_x, target_y)) {
            setPos(target_x, target_y);
        }
        else {
            target_y = y();
            int currentDirection = getDirection();
            (currentDirection == 0) ? target_x += 2 : target_x -= 2;
            if (world()->obstacleAt(target_x, target_y)) {
                (currentDirection == 0) ? setDirection(180) : setDirection(0);
            }
            else {
                setPos(target_x, target_y);
            }
        }
    }
}

//================================================== GOODIE VARIATIONS ==================================================//

Flower::Flower(StudentWorld* StudentWorld, int x, int y) : Goodie(StudentWorld, IID_FLOWER, flower, x, y) {}

Mushroom::Mushroom(StudentWorld* StudentWorld, int x, int y) : Goodie(StudentWorld, IID_MUSHROOM, mushroom, x, y) {}

Star::Star(StudentWorld* StudentWorld, int x, int y) : Goodie(StudentWorld, IID_STAR, star, x, y) {}
