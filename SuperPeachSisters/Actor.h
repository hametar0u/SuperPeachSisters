#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* StudentWorld, int imageID, int startX, int startY, int startDirection = 0, int depth = 0, int size = 1);
    virtual ~Actor() {}
    
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
    StudentWorld* m_StudentWorld;
};

//================================================== PEACH ==================================================//

class Peach : public Actor {
public:
    Peach(StudentWorld* StudentWorld, int x, int y);
    virtual ~Peach() {}
    
    virtual void doSomething();
private:
    int m_hp;
    bool m_invincible;
    std::vector<std::string> m_powerups; //tbh there might be a fixed # of powerups so might be better to implement an array of bools
};

class Obstacle : public Actor {
public:
    Obstacle(StudentWorld* StudentWorld, int imageID, int startX, int startY);
    virtual ~Obstacle() {}
    
    virtual void blockPath();
    virtual void doSomething();
};

//================================================== OBSTACLES ==================================================//

class Block : public Obstacle {
public:
    Block(StudentWorld* StudentWorld, int x, int y);
    virtual ~Block() {}
    
    virtual void getBonked();
    //TODO: deal with the goodie stuff
private:
    bool wasBonked;
};

class Pipe : public Obstacle {
public:
    Pipe(StudentWorld* StudentWorld, int x, int y);
    virtual ~Pipe() {}
};





#endif // ACTOR_H_
