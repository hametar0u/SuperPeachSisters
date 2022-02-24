#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* StudentWorld, int imageID, int startX, int startY, int startDirection = 0, int depth = 0, int size = 1);
    virtual ~Actor() {}
    
    //predicates
    bool isAlive() const { return m_isAlive; }
    bool isAt(int x, int y) const;
    virtual bool isDamageable() const { return false; }
    
    //getters
    StudentWorld* world() { return m_StudentWorld; }
    
    //setters
    void toggleAlive() { m_isAlive = !m_isAlive; }
    void setPos(int new_x, int new_y);
    
    //other
    virtual void doSomething() = 0;
    virtual void bonk() { return; }
    virtual void damage() { return; }
    virtual bool blocksMovement() { return false; }
    
private:
    bool m_isAlive;
    StudentWorld* m_StudentWorld;
    
};

//================================================== PEACH ==================================================//

class Peach : public Actor {
public:
    Peach(StudentWorld* StudentWorld, int x, int y);
    virtual ~Peach() {}
    
    virtual bool isDamageable() const { return true; }
    
    virtual void doSomething();
    virtual void bonk();
    virtual void damage() { bonk(); }
    
    bool hasBuff(std::string buff);
    void addBuff(std::string buff) { m_powerups.insert(buff); }
    void setHealth(int amt) { m_hp = amt; }
    
private:
    int m_hp;
    int remaining_invincibility;
    int remaining_temporary_invincibility;
    int remaining_jump_distance;
    int time_to_recharge_before_next_fire;
    std::set<std::string> m_powerups; //tbh there might be a fixed # of powerups so might be better to implement an array of bools
};

//================================================== FLAGS ==================================================//

class Flag : public Actor {
public:
    Flag(StudentWorld* StudentWorld, int startX, int startY, int imageID = IID_FLAG);
    virtual ~Flag() {}
    
    virtual void doSomething();
    virtual void progressNext();
};

class Mario : public Flag {
public:
    Mario(StudentWorld* StudentWorld, int x, int y);
    virtual ~Mario() {}
    
    virtual void progressNext();
    
};

//================================================== GOODIES ==================================================//
enum GoodieType { flower, mushroom, star, none };

class Goodie : public Actor {
public:
    Goodie(StudentWorld* StudentWorld, int imageID, GoodieType goodie, int x, int y);
    virtual ~Goodie() {}
    
    virtual void doSomething();
    
private:
    GoodieType m_goodie;
};

class Flower : public Goodie {
public:
    Flower(StudentWorld* StudentWorld, int x, int y);
    virtual ~Flower() {}
};

class Mushroom : public Goodie {
public:
    Mushroom(StudentWorld* StudentWorld, int x, int y);
    virtual ~Mushroom() {}
};

class Star : public Goodie {
public:
    Star(StudentWorld* StudentWorld, int x, int y);
    virtual ~Star() {}
};

//================================================== OBSTACLES ==================================================//

class Obstacle : public Actor {
public:
    Obstacle(StudentWorld* StudentWorld, int imageID, int startX, int startY);
    virtual ~Obstacle() {}
    
    virtual bool blocksMovement() { return true; }
    virtual void doSomething();
};

class Block : public Obstacle {
public:
    Block(StudentWorld* StudentWorld, int x, int y, GoodieType goodie);
    virtual ~Block() {}
    
    //goodies
    bool hasGoodie() const { return m_goodie != none; };
    void releaseGoodie();
    
    virtual void bonk();
private:
    GoodieType m_goodie;
};

class Pipe : public Obstacle {
public:
    Pipe(StudentWorld* StudentWorld, int x, int y);
    virtual ~Pipe() {}
};

//================================================== ENEMIES ==================================================//

class Enemy : public Actor {
public:
    Enemy(StudentWorld* StudentWorld, int imageID, int startX, int startY);
    virtual ~Enemy() {}
    
    virtual bool isDamageable() const { return true; }
    
    virtual void doSomething();
    virtual void bonk();
    virtual void damage();
};

class Goomba : public Enemy {
public:
    Goomba(StudentWorld* StudentWorld, int startX, int startY, int imageID = IID_GOOMBA);
    virtual ~Goomba() { cerr << "destroying goomba" << endl; }
};

class Koopa : public Goomba {
public:
    Koopa(StudentWorld* StudentWorld, int startX, int startY);
    virtual ~Koopa() { cerr << "destroying koopa" << endl; }
};

class Piranha : public Enemy {
public:
    Piranha(StudentWorld* StudentWorld, int startX, int startY);
    virtual ~Piranha() { cerr << "destroying piranha" << endl; }
    
    virtual void doSomething();
    
private:
    int m_firing_delay;
};

//================================================== PROJECTILES ==================================================//

class Projectile : public Actor {
public:
    Projectile(StudentWorld* StudentWorld, int imageID, int x, int y, int startDirection);
    virtual ~Projectile() {}
    
    virtual void doSomething();
    
};

//TODO: implement
class PiranhaFire : public Projectile {
public:
    PiranhaFire(StudentWorld* StudentWorld, int x, int y, int startDirection);
    virtual ~PiranhaFire() { cerr << "destroying piranhafire " << endl; }
    
};

class PeachFire : public Projectile {
public:
    PeachFire(StudentWorld* StudentWorld, int x, int y, int startDirection);
    virtual ~PeachFire() { cerr << "destroying peachfire " << endl; }
};
class Shell : public Projectile {
public:
    Shell(StudentWorld* StudentWorld, int x, int y, int startDirection);
    virtual ~Shell() { cerr << "destroying shell " << endl; }
};


#endif // ACTOR_H_
