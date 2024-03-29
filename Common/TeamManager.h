#ifndef TeamManager_h_
#define TeamManager_h_

#include "Team.h"
#include "Images.h"
#include <iostream>
#include <vector>
namespace tg
{
    class TeamManager
    {
    public:
        TeamManager(){
            //Get space for all our people seats
            teams.reserve(3);
            teams.push_back(Team());//Team Limbo
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());
            teams.back().players.push_back(Player());

            teams.push_back(Team());//Team 1
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 0;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 1;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 2;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 3;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 4;

            teams.push_back(Team());//Team 2
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 0;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 1;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 2;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 3;
            teams.back().players.push_back(Player());teams.back().players.back().slotNum = 4;
        }

        sf::Uint32 load()
        {
            return 0;
        }

        sf::Uint32 addPlayer(tg::Player & p, std::string name,  int team){
            for (int i = 0;i < 5;i++){
                if (teams[team].players[i].hasHost == 0){
                    teams[team].players[i] = p;
                    teams[team].players[i].playerName = name;
                    teams[team].players[i].slotNum = i;
                    teams[team].players[i].hasHost = 1;
                    return i;
                }
            }
            return -1;//No team had less than 5 players..
        }
        Player & getPlayerBySlot(int team, int slot){
            return teams[team].players[slot];
        }

        tg::Player & getPlayer(int cid){
            for (int i = 0;i < 3;i++){
                for (auto y = teams[i].players.begin();y != teams[i].players.end();y++){
                    if (y->hasHost == 1 && y->connectionId == cid)
                        return *y;
                }
            }
            std::cout << "WARNING: getPlayer() didnt' return anything; this function sucks" <<std::endl;
        }

       
        bool removePlayer(int cid){
            for (int i = 0;i < 3;i++){
                for (auto y = teams[i].players.begin();y != teams[i].players.end();y++){
                    if (y->hasHost == 1 && y->connectionId == cid){
                        y->hasHost = 0;
                        y->playerName = "-removed-";
                        return true;
                    }
                }
            }
            return false;
        }

        bool isIdValid(std::string name, int team)
        {
            if (isNameValid(name)){
                if (isTeamValid(team)){
                    return true;
                }
            }
            return false;
        }

        bool isNameValid(std::string name)
        {
            for (int i = 1;i < 3;i++){
                for (auto y = teams[i].players.begin();y != teams[i].players.end();y++){
                    if (y->hasHost == 1 && y->playerName == name)
                        return false;
                }
            }
            return true;
        }
        bool isTeamValid(int team)
        {
            if (team < 1 || team > 2)
                return false;
            for (int i = 0;i < 5;i++){
                if (teams[team].players[i].hasHost == 0)
                    return true;
            }
            return false;
        }

        std::vector<Player> &  getTeam(sf::Uint32 tm){
            return teams[tm].players;
        }
    //private:
        std::vector<Team> teams;
        std::list<Explosion> explosions;
    };
};

#endif


