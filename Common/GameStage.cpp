#include "GameStage.h"

#include "LogFile.h"
#include "Comm.h"
#include "TeamManager.h"
#include "ArenaManager.h"
#include "AssetManager.h"

using namespace tg;

GameStage::GameStage()
{
    Element e;
    e.a = 0;
    setSummary(e,0);
}
GameStage::~GameStage()
{

}

sf::Uint32 GameStage::doInit(Game & g)
{
    //LogFile::get()->log(0,0,"GameStage::doInit");
    return 0;
}

sf::Uint32 GameStage::doRemoteEvent(Game & g,
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



sf::Uint32 GameStage::doLocalInput(sf::RenderWindow &window, Game & g)
{
    //LogFile::get()->log(0,0,"GameStage::doLocalInput");
    return 0;
}

sf::Uint32 GameStage::doLoop(Game & g)
{
    //LogFile::get()->log(0,0,"GameStage::doLoop");
    return 0;
}

sf::Uint32 GameStage::doCleanup(Game & g)
{
    //LogFile::get()->log(0,0,"GameStage::doCleanup");
    return 0;
}

sf::Uint32 GameStage::doDraw(sf::RenderWindow &window,Game & g, sf::Time ft)
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

void GameStage::setSummary(Element e, sf::Uint16 i)
{
    //LogFile::get()->log(0,0,"GameStage::setSummary");
    summary[i] = e;
}

Element GameStage::getSummary(sf::Uint16 i)
{
    //LogFile::get()->log(0,0,"GameStage::getSummary");
    return summary[i];
}


void GameStage::setInput(Element e, sf::Uint16 i)
{
    //LogFile::get()->log(0,0,"GameStage::setInput");
    inputs[i] = e;
}

Element GameStage::getInput(sf::Uint16 i)
{
    //LogFile::get()->log(0,0,"GameStage::getSummary");
    return inputs[i];
}
