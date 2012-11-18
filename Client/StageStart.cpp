#include "StageStart.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"

using namespace tg;

StageStart::StageStart()
    : GameStage()
{

}

sf::Uint32 StageStart::doRemoteEvent(TeamManager & teamMan, 
                                     ArenaManager & arenaMan,
                                     CommEvent & cevent,
                                     sf::Uint32 connId,
                                     sf::Uint32 msgId)
{


    return 0;
}

sf::Uint32 StageStart::doLoop(Comm & comm, TeamManager & teamMan)
{
    setSummary(1,0);
    return getSummary(0);
}
sf::Uint32 StageStart::doLocalInput(TeamManager & teamMan)
{
    return 0;
}