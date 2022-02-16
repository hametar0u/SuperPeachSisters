#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();

private:
    void displayObjectAt(Level::GridEntry ge, int x, int y);
    
    std::vector<Actor*> m_actors;
    //I think I should have a map of positions to Actor*
    Peach* m_Peach;
};

#endif // STUDENTWORLD_H_
