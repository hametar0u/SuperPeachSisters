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
    bool objectAt(int x, int y);
    bool obstacleAt(int x, int y);
    bool obstacleBelow(int x, int y);
    void bonkObjectsAt(int x, int y);
    bool overlapsWithPeach(int x, int y);
    void finishLevel() { level_finished = true; }

private:
    void displayObjectAt(Level::GridEntry ge, int x, int y);
    
    std::vector<Actor*> m_actors;
    Peach* m_Peach;
    
    bool level_finished;
};

#endif // STUDENTWORLD_H_
