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
    
    //helpers
    bool objectAt(int x, int y);
    void bonkObjectsAt(int x, int y);

private:
    void displayObjectAt(Level::GridEntry ge, int x, int y);
    
    std::vector<Actor*> m_actors;
    
//    Actor* m_coordmap[VIEW_WIDTH][VIEW_HEIGHT];
    //debating between storing positions in studentWorld or in each individual actor.
    //if studentWorld, use 2D array to store actors' at the specified [x][y] index. Advantages: O(1) lookup since that happens quite a bit. Disadvantages: have to modify the positions of the pointers a lot
    //if actor, store position as member variables, with public accessor methods. Advantages: modifying positions a lot easier. Disadvantages: O(n) lookup on average if want to determine if some actor is at a certain position
    Peach* m_Peach;
};

#endif // STUDENTWORLD_H_
