#include "StageStart.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"

using namespace tg;

StageStart::StageStart()
    : GameStage()
{

}

sf::Uint32 StageStart::doRemoteEvent(Game & g,
                                     CommEvent & cevent,
                                     sf::Uint32 connId,
                                     sf::Uint32 msgId)
{

    return 0;
}

sf::Uint32 StageStart::doLoop(Game & g)
{
    setSummary(1,0);
    return getSummary(0);
}
sf::Uint32 StageStart::doLocalInput(sf::RenderWindow & window, Game & g)
{
    return 0;
}