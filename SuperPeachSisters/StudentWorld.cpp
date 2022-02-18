#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_actors = {};
    m_Peach = nullptr;
    level_finished = false;
}

int StudentWorld::init()
{
    Level lev(assetPath());
    
    ostringstream oss;
    oss << "level0" << getLevel() << ".txt";
    
    string level_file = oss.str(); //TODO: use stringstream to generate filename instead of hard coding like this
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found) {
        cerr << "Could not find level01.txt data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }

    else if (result == Level::load_fail_bad_format) {
        cerr << "level01.txt is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;
        Level::GridEntry ge;
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                ge = lev.getContentsOf(x, y);
                displayObjectAt(ge, x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    m_Peach->doSomething();
    for (Actor* actor : m_actors) {
        actor->doSomething();
    }
    
    if (level_finished) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if (game_finished) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    if (getLives() > 0)
        return GWSTATUS_CONTINUE_GAME;
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    for (Actor* actor : m_actors) {
        delete actor;
    }
    delete m_Peach;
    
    m_Peach = nullptr;
    m_actors.resize(0); //is this allowed lol
    
    level_finished = false;
}


//helper functions
void StudentWorld::displayObjectAt(Level::GridEntry ge, int x, int y) {
    switch (ge) {
        case Level::empty:
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::koopa:
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::goomba:
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::peach:
//            cerr << "peach at " << x << "," << y << endl;
            m_Peach = new Peach(this, x, y);
            break;
        case Level::flag:
//            cerr << "flag at " << x << "," << y << endl;
            m_actors.push_back(new Flag(this, x, y));
            break;
        case Level::block:
//            cerr << "block at " << x << "," << y << endl;
            m_actors.push_back(new Block(this, x, y));
            
            break;
        case Level::star_goodie_block:
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::piranha:
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::mushroom_goodie_block:
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::flower_goodie_block:
//            cerr << "flower block" << x << "," << y << endl;
            m_actors.push_back(new Block(this, x, y, IID_FLOWER));
            break;
        case Level::pipe:
//            cerr << "pipe at " << x << "," << y << endl;
            m_actors.push_back(new Pipe(this, x, y));
            break;
        case Level::mario:
//            cerr << "block at" << x << "," << y << endl;
            m_actors.push_back(new Mario(this, x, y));
            break;
    }
}

bool StudentWorld::objectAt(int x, int y) const {
    for (Actor* actor : m_actors) {
        if (actor->isAt(x, y)) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::obstacleAt(int x, int y) const {
    for (Actor* actor : m_actors) {
        if (actor->isAt(x, y) && actor->blocksMovement()) {
            return true;
        }
    }
    return false;
}

void StudentWorld::bonkObjectsAt(int x, int y) {
    for (Actor* actor : m_actors) {
        if (actor->isAt(x, y)) {
            actor->bonk();
        }
    }
}

bool StudentWorld::obstacleBelow(int x, int y) const {
    bool obstacleBelow = false;
    for (int i = 1; i < 4; i++) {
        if (obstacleAt(x, y - i))
            obstacleBelow = true;
    }
    return obstacleBelow;
}

bool StudentWorld::overlapsWithPeach(int x, int y) const {
    if (m_Peach->isAt(x, y))
        return true;
    return false;
}

void StudentWorld::buffPeach(string buff) {
    m_Peach->addBuff(buff);
}

void StudentWorld::createActor(int typeOfActor, int x, int y) { //TODO: this should mostly be used for powerups but I'll keep it general for now
    switch (typeOfActor) {
        case IID_FLOWER:
            m_actors.push_back(new Flower(this, x, y));
            break;
            //TODO: add the other goodies
    }
}
