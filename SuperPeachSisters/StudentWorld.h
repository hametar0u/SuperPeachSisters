#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
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
    bool obstacleAt(int x, int y) const;
    bool obstacleBelow(int x, int y) const;
    void bonkObjectsAt(int x, int y);
    bool overlapsWithPeach(int x, int y) const;
    
    //peach functions
    void buffPeach(std::string buff);
    void setPeachHealth(int amt);
    
    //actor manipulation
    void createActor(int typeOfActor, int x, int y);
    //note: to delete an actor, just iterate through m_actors on each cycle, check alive/death status, and delete accordingly
    
    void finishLevel() { level_finished = true; }
    void endGame() { game_finished = true; }

private:
    void displayObjectAt(Level::GridEntry ge, int x, int y);
    
    std::vector<Actor*> m_actors;
    Peach* m_Peach;
    
    bool level_finished; //TODO: come back to these
    bool game_finished;
};

#endif // STUDENTWORLD_H_
