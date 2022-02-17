#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <set>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* StudentWorld, int imageID, int startX, int startY, int startDirection = 0, int depth = 0, int size = 1);
    virtual ~Actor() {}
    
    //predicates
    bool isAlive() { return m_isAlive; }
    bool isAt(int x, int y);
    
    //getters
    StudentWorld* world() { return m_StudentWorld; }
    
    int x() { return m_x; }
    int y() { return m_y; }
    
    //setters
    void toggleAlive() { m_isAlive = !m_isAlive; }
    void setX(int new_x); // TODO: remove these if we don't need them
    void setY(int new_y);
    void setPos(int new_x, int new_y);
    
    //other
    virtual void doSomething() = 0;
    virtual void bonk() { return; }
    virtual bool blocksMovement() { return false; }
    
private:
    bool m_isAlive;
    StudentWorld* m_StudentWorld;
    
    //position of actor
    int m_x;
    int m_y;
};

//================================================== PEACH ==================================================//

class Peach : public Actor {
public:
    Peach(StudentWorld* StudentWorld, int x, int y);
    virtual ~Peach() {}
    
    virtual void doSomething();
    virtual void bonk();
    
private:
    int m_hp;
    int remaining_invincibility;
    int remaining_temporary_invincibility;
    int remaining_jump_distance;
    int time_to_recharge_before_next_fire;
    std::set<std::string> m_powerups; //tbh there might be a fixed # of powerups so might be better to implement an array of bools
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
    Block(StudentWorld* StudentWorld, int x, int y);
    virtual ~Block() {}
    
    virtual void bonk();
    //TODO: deal with the goodie stuff
private:
    bool wasBonked;
};

class Pipe : public Obstacle {
public:
    Pipe(StudentWorld* StudentWorld, int x, int y);
    virtual ~Pipe() {}
};

//================================================== FLAGS ==================================================//

class Flag : public Actor {
public:
    Flag(StudentWorld* StudentWorld, int startX, int startY);
    virtual ~Flag() {}
    
    void doSomething(); //??
    virtual void progressNext();
};

class Mario : public Flag {
public:
    Mario(StudentWorld* StudentWorld, int x, int y);
    virtual ~Mario() {}
    
    virtual void progressNext();
    
};

#endif // ACTOR_H_
