#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int startDirection = 0, int depth = 0, int size = 1);
    ~Actor();
    
    //getters
    bool isAlive() { return m_isAlive; }
    
    //setters
    void toggleAlive() { m_isAlive = !m_isAlive; }
    
    //other
    virtual void doSomething() = 0;
    virtual void getBonked() { return; }
    virtual void blockPath() { return; }
    
private:
    bool m_isAlive;
};

class Obstacle : public Actor {
public:
    Obstacle(int imageID);
    ~Obstacle();
    
    virtual void blockPath();
};

class Block : public Obstacle {
public:
    Block();
    ~Block();
    
    virtual void doSomething();
    virtual void getBonked();
    //TODO: deal with the goodie stuff
private:
    bool wasBonked;
};

#endif // ACTOR_H_
