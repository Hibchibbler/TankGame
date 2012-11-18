#include "GameStage.h"

#include "LogFile.h"
#include "Comm.h"
#include "TeamManager.h"
#include "ArenaManager.h"

using namespace tg;

GameStage::GameStage()
{
    setSummary(0,0);
}
GameStage::~GameStage()
{

}

sf::Uint32 GameStage::doRemoteEvent(TeamManager & teamMan, 
                                    ArenaManager & arenaMan,
                                    CommEvent & cevent,
                                    sf::Uint32 connId,
                                    sf::Uint32 msgId)
{
    //LogFile::get()->log(0,0,"GameStage::doRemoteEvent");
    return 0;
}

sf::Uint32 GameStage::doWindowEvent(sf::RenderWindow & w, sf::Event & event)
{
    //LogFile::get()->log(0,0,"GameStage::doWindowEvent");
    return 0;
}

sf::Uint32 GameStage::doInit(int w, int h)
{
    //LogFile::get()->log(0,0,"GameStage::doInit");
    return 0;
}

sf::Uint32 GameStage::doLocalInput(TeamManager & teamMan)
{
    //LogFile::get()->log(0,0,"GameStage::doLocalInput");
    return 0;
}

sf::Uint32 GameStage::doLoop(Comm & comm, TeamManager & teamMan)
{
    //LogFile::get()->log(0,0,"GameStage::doLoop");
    return 0;
}

sf::Uint32 GameStage::doCleanup()
{
    //LogFile::get()->log(0,0,"GameStage::doCleanup");
    return 0;
}

sf::Uint32 GameStage::doDraw(sf::RenderWindow &window)
{
    //LogFile::get()->log(0,0,"GameStage::doDraw");
    return 0;
}
    
void GameStage::setId(sf::Uint32 a)
{
    //LogFile::get()->log(0,0,"GameStage::setId");
    id = a; 
}
sf::Uint32 GameStage::getId()
{
    //LogFile::get()->log(0,0,"GameStage::getId");
    return id; 
}

void GameStage::setSummary(sf::Uint32 s, sf::Uint16 i)
{
    LogFile::get()->log(0,0,"GameStage::setSummary");
    summary[i] = s;
}

sf::Uint32 GameStage::getSummary(sf::Uint16 i)
{
    //LogFile::get()->log(0,0,"GameStage::getSummary");
    return summary[i];
}


void GameStage::setInput(sf::Uint32 s, sf::Uint16 i)
{
    LogFile::get()->log(0,0,"GameStage::setInput");
    inputs[i] = s;
}

sf::Uint32 GameStage::getInput(sf::Uint16 i)
{
    //LogFile::get()->log(0,0,"GameStage::getSummary");
    return inputs[i];
}
