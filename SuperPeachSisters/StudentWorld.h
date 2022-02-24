#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <list>
#include <string>
#include <map>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld() { cleanUp(); }
    
    //helpers
    bool objectAt(int x, int y) const;
    bool damageableObjectAt(int x, int y) const;
    bool obstacleAt(int x, int y) const;
    void bonkObjectsAt(int x, int y) const;
    bool overlapsWithPeach(int x, int y) const;
    bool onSameLevelAsPeach(int x, int y, bool& onLeft);
    bool peachInRange(int x) const;
    
    //peach functions
    bool peachIsInvincible() { return m_Peach->hasBuff("StarPower"); }
    
    void bonkPeach() { m_Peach->bonk(); }
    void damagePeach() { m_Peach->damage(); }
    void buffPeach(std::string buff);
    void setPeachHealth(int amt);
    
    //actor manipulation
    void createActor(int typeOfActor, int x, int y, int startDirection = 0);
    //note: to delete an actor, just iterate through m_actors on each cycle, check alive/death status, and delete accordingly
    
    void finishLevel() { level_finished = true; }
    void endGame() { game_finished = true; }

private:
    void displayObjectAt(Level::GridEntry ge, int x, int y);
    
    list<Actor*> m_actors;
    Peach* m_Peach;
    
    bool level_finished; //TODO: come back to these
    bool game_finished;
};

#endif // STUDENTWORLD_H_
