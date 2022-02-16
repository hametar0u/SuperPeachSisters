#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
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
//    for (int x = 0; x < VIEW_WIDTH; x++) {
//        for (int y = 0; y < VIEW_HEIGHT; y++) {
//            m_coordmap[x][y] = nullptr;
//        }
//    }
}

//int StudentWorld::init() {
//    m_actors.push_back(new Block());
//    cerr << "do console logs even do anything" << endl; //console log doesn't do anything
//    return GWSTATUS_CONTINUE_GAME;
//}

int StudentWorld::init()
{
    Level lev(assetPath());

    string level_file = "level01.txt"; //TODO: use stringstream to generate filename instead of hard coding like this
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
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* actor : m_actors) {
        delete actor;
    }
    delete m_Peach;
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
            cerr << "peach at " << x << "," << y << endl;
            m_Peach = new Peach(this, x, y);
            break;
        case Level::flag:
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::block:
            cerr << "block at " << x << "," << y << endl;
            m_actors.push_back(new Block(this, x, y));
//            m_coordmap[x][y] = new Block(this, x, y);
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
            cerr << "block at" << x << "," << y << endl;
            break;
        case Level::pipe:
            cerr << "pipe at " << x << "," << y << endl;
            m_actors.push_back(new Pipe(this, x, y));
            break;
        case Level::mario:
            cerr << "block at" << x << "," << y << endl;
            break;
    }
}

bool StudentWorld::objectAt(int x, int y) {
    for (Actor* actor : m_actors) {
        if (actor->isAt(x, y)) {
            return true;
        }
    }
    return false;
}

void StudentWorld::bonkObjectsAt(int x, int y) {
    for (Actor* actor : m_actors) {
        if (actor->isAt(x, y)) {
            actor->getBonked();
        }
    }
}
