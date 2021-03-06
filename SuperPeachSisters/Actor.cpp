#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//================================================== ACTOR ==================================================//

Actor::Actor(StudentWorld* StudentWorld, int imageID, int startX, int startY, int startDirection, int depth, int size)
: GraphObject(imageID, startX, startY, startDirection, depth, size) {
    m_isAlive = true;
    m_StudentWorld = StudentWorld;
    moveTo(startX, startY);
}

bool Actor::isAt(int x, int y) const {
    return min(getX(), static_cast<double>(x)) + SPRITE_WIDTH > max(getX(), static_cast<double>(x))
        && min(getY(), static_cast<double>(y)) + SPRITE_HEIGHT > max(getY(), static_cast<double>(y));
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
    if (remaining_invincibility > 0) {
        remaining_invincibility--;
        if (remaining_invincibility == 0 && hasBuff("StarPower"))
            m_powerups.erase(m_powerups.find("StarPower"));
    }
    if (hasBuff("StarPower") && remaining_invincibility == 0)
        remaining_invincibility = 300;
    if (remaining_temporary_invincibility > 0)
        remaining_temporary_invincibility--;
    if (time_to_recharge_before_next_fire > 0)
        time_to_recharge_before_next_fire--;
    if (world()->objectAt(getX(), getY()))
        world()->bonkObjectsAt(getX(), getY());
    
    int target_x = getX();
    int target_y = getY();
    
    if (remaining_jump_distance > 0) {
        target_y += 4;
        if (world()->obstacleAt(target_x, target_y)) {
            world()->bonkObjectsAt(target_x, target_y);
            remaining_jump_distance = 0;
        }
        else {
            moveTo(target_x, target_y);
            remaining_jump_distance--;
        }
    }
    else {
        
        if (!world()->obstacleAt(target_x, target_y - 4)) {
            target_y -= 4;
            moveTo(target_x, target_y);
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
                if (world()->obstacleAt(target_x, target_y - 3)) {
                    if (m_powerups.find("JumpPower") != m_powerups.end()) {
                        remaining_jump_distance = 12;
                    }
                    else {
                        remaining_jump_distance = 8;
                    }
                    world()->playSound(SOUND_PLAYER_JUMP);
                    return;
                }
                break;
            case KEY_PRESS_SPACE:
                if (m_powerups.find("ShootPower") != m_powerups.end() && time_to_recharge_before_next_fire <= 0) {
                    world()->playSound(SOUND_PLAYER_FIRE);
                    time_to_recharge_before_next_fire = 8;
                    world()->createActor(IID_PEACH_FIRE, getX(), getY(), getDirection());
                }
                break;
        }
        
        if (world()->obstacleAt(target_x, target_y)) {
            world()->bonkObjectsAt(target_x, target_y);
            return;
        }
        else {
            moveTo(target_x, target_y);
        }
    }
}

void Peach::bonk() {
    if (remaining_invincibility > 0 || remaining_temporary_invincibility > 0)
        return;
    
    m_hp--;
    remaining_temporary_invincibility = 10;
    if (hasBuff("ShootPower"))
        m_powerups.erase("ShootPower");
    if (hasBuff("JumpPower"))
        m_powerups.erase("JumpPower");
    if (m_hp > 0)
        world()->playSound(SOUND_PLAYER_HURT);
    if (m_hp <= 0) {
        world()->decLives();
        toggleAlive();
    }
}

bool Peach::hasBuff(string buff) {
    return m_powerups.find(buff) != m_powerups.end() ? true : false;
}

//================================================== OBSTACLES ==================================================//

Obstacle::Obstacle(StudentWorld* StudentWorld, int imageID, int startX, int startY) : Actor(StudentWorld, imageID, startX, startY, 0, 2 /*default size*/) {}

void Obstacle::doSomething() { return; } //it's supposed to do nothing

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
    int target_x = getX();
    int target_y = getY() + 8;
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

Pipe::Pipe(StudentWorld* StudentWorld, int x, int y) : Obstacle(StudentWorld, IID_PIPE, x, y) {}

//================================================== FLAGS ==================================================//

Flag::Flag(StudentWorld* StudentWorld, int startX, int startY, int imageID) : Actor(StudentWorld, imageID, startX, startY) {}

void Flag::doSomething() {
    if (!isAlive())
        return;
    if (world()->overlapsWithPeach(getX(), getY())) {
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
    if (world()->overlapsWithPeach(getX(), getY())) {
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
        world()->setPeachHealth(2);
        toggleAlive();
        world()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    else {
        int target_x = getX();
        int target_y = getY() - 2;
        if (!world()->obstacleAt(target_x, target_y)) {
            moveTo(target_x, target_y);
        }
        else {
            target_y = getY();
            int currentDirection = getDirection();
            (currentDirection == 0) ? target_x += 2 : target_x -= 2;
            if (world()->obstacleAt(target_x, target_y)) {
                (currentDirection == 0) ? setDirection(180) : setDirection(0);
            }
            else {
                moveTo(target_x, target_y);
            }
        }
    }
}

//================================================== GOODIE VARIATIONS ==================================================//

Flower::Flower(StudentWorld* StudentWorld, int x, int y) : Goodie(StudentWorld, IID_FLOWER, flower, x, y) {}

Mushroom::Mushroom(StudentWorld* StudentWorld, int x, int y) : Goodie(StudentWorld, IID_MUSHROOM, mushroom, x, y) {}

Star::Star(StudentWorld* StudentWorld, int x, int y) : Goodie(StudentWorld, IID_STAR, star, x, y) {}

//================================================== ENEMIES ==================================================//

Enemy::Enemy(StudentWorld* StudentWorld, int imageID, int startX, int startY) : Actor(StudentWorld, imageID, startX, startY, randInt(0, 1) * 180) {}

void Enemy::doSomething() {
    if (!isAlive())
        return;
    if (world()->overlapsWithPeach(getX(), getY())) {
        world()->bonkPeach();
        return;
    }
    
    int currentDirection = getDirection();
    int target_x = getX();
    int target_y = getY();
    (currentDirection == 0) ? target_x += 1 : target_x -= 1;
    
    if (world()->obstacleAt(target_x, target_y))
        (currentDirection == 0) ? setDirection(180) : setDirection(0);
    
    //determine if theres a block one pixel in the direction of enemy and one block down
    target_x = getX();
    (currentDirection == 0) ? target_x += SPRITE_WIDTH : target_x -= SPRITE_WIDTH;
    target_y -= SPRITE_HEIGHT;
    
    if (!world()->obstacleAt(target_x, target_y))
        (currentDirection == 0) ? setDirection(180) : setDirection(0);
    
    currentDirection = getDirection();
    target_x = getX();
    target_y = getY();
    (currentDirection == 0) ? target_x += 1 : target_x -= 1;
    if (world()->obstacleAt(target_x, target_y))
        return;
    
    moveTo(target_x, target_y);
}

void Enemy::bonk() {
    //only peach will ever bonk objects so this is fine
    if (world()->peachIsInvincible()) {
        world()->playSound(SOUND_PLAYER_KICK);
        damage();
    }
}

void Enemy::damage() {
    world()->increaseScore(100);
    toggleAlive();
}

Goomba::Goomba(StudentWorld* StudentWorld, int startX, int startY, int imageID) : Enemy(StudentWorld, imageID, startX, startY) {}

Koopa::Koopa(StudentWorld* StudentWorld, int startX, int startY) : Goomba(StudentWorld, startX, startY, IID_KOOPA) {}

void Koopa::damage() {
    Enemy::damage();
    world()->createActor(IID_SHELL, getX(), getY(), getDirection());
}

Piranha::Piranha(StudentWorld* StudentWorld, int startX, int startY) : Enemy(StudentWorld, IID_PIRANHA, startX, startY) {
    m_firing_delay = 0;
}

void Piranha::doSomething() {
    if (!isAlive())
        return;
    increaseAnimationNumber();
    
    if (world()->overlapsWithPeach(getX(), getY())) {
        world()->bonkPeach();
        return;
    }
    
    bool onLeft;
    if (!world()->onSameLevelAsPeach(getX(), getY(), onLeft))
        return;
    onLeft == true ? setDirection(180) : setDirection(0);
    
    if (m_firing_delay > 0) {
        m_firing_delay--;
        return;
    }
    if (world()->peachInRange(getX())) {
        world()->createActor(IID_PIRANHA_FIRE, getX(), getY(), getDirection());
        world()->playSound(SOUND_PIRANHA_FIRE);
        m_firing_delay = 40;
    }
}

//================================================== PROJECTILES ==================================================//

Projectile::Projectile(StudentWorld* StudentWorld, int imageID, int x, int y, int startDirection) : Actor(StudentWorld, imageID, x, y, startDirection, 1) {}

void Projectile::doSomething() {
    if (overlapsWithTarget(getX(), getY())) {
        damageTarget(getX(), getY());
        toggleAlive();
        return;
    }
    
    int target_x = getX();
    int target_y = getY() - 2;
    if (!world()->obstacleAt(target_x, target_y)) {
        moveTo(target_x, target_y);
    }
    
    (getDirection() == 0) ? target_x += 2 : target_x -= 2;
    target_y = getY();
    
    if (world()->obstacleAt(target_x, target_y)) {
        toggleAlive();
        return;
    }
    else {
        moveTo(target_x, target_y);
    }
}

bool Projectile::overlapsWithTarget(int x, int y) {
    return world()->damageableObjectAt(x, y);
}

void Projectile::damageTarget(int x, int y) {
    world()->damageObjectsAt(x, y);
}

PiranhaFire::PiranhaFire(StudentWorld* StudentWorld, int x, int y, int startDirection) : Projectile(StudentWorld, IID_PIRANHA_FIRE, x, y, startDirection) {}

bool PiranhaFire::overlapsWithTarget(int x, int y) {
    return world()->overlapsWithPeach(x, y);
}

void PiranhaFire::damageTarget(int x, int y) {
    world()->damagePeach();
}

PeachFire::PeachFire(StudentWorld* StudentWorld, int x, int y, int startDirection) : Projectile(StudentWorld, IID_PEACH_FIRE, x, y, startDirection) {}

Shell::Shell(StudentWorld* StudentWorld, int x, int y, int startDirection) : Projectile(StudentWorld, IID_SHELL, x, y, startDirection) {}
