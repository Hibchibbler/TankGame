/**********************************************************************************
Tank Game
ArenaManager.h

Copyright 2012
Daniel Ferguson
**********************************************************************************/

#ifndef ArenaManager_h_
#define ArenaManager_h_

#include <SFML/Graphics.hpp>

#define ARENAMAN_TILE_WIDTH  125
#define ARENAMAN_TILE_HEIGHT 125

#include <iostream>
#include <fstream>
#include <vector>

namespace tg
{
    class Tile
    {
    public:
        void setId(sf::Uint32 i){
            id = i;
        }

        void setPosition(sf::Vector2f pos){
            position = pos;
        }

        sf::Vector2f getPosition(){
            return position;
        }

        sf::Uint32 getId(){
            return id;
        }

        void setName(std::string n){name=n;}
        std::string getName(){return name;}
    private:
        sf::Vector2f position;
        sf::Uint32 id;
        std::string name;
    };

    class AssetManager;
    class ArenaManager
    {
    public:
        sf::Uint32 load(std::string fn){

            //Open Map data file
            std::ifstream fin(fn,std::ios::in|std::ios::binary  );
            if (fin.is_open()){
                //First two dwords are width and height respectively
                fin.read((char*)&horizTileNum, 4);
                fin.read((char*)&vertTileNum, 4);

                //Now populate arena vector
                //Although we intend to display a grid
                //the sprites are laid out in contiguous memory.
                //So, we have index, row, and %, helping us translate
                //index to x&y
                int index = 0;
                int row = 0;
                arenaData.reserve(horizTileNum*vertTileNum);
                while (!fin.eof()){
                    if (index == horizTileNum*vertTileNum){
                        int a = 42;
                    }

                    sf::Uint32 id;
                    fin.read((char*)&id, 4);
                    arenaData.push_back(Tile());
                    arenaData[index].setId(id);

                    //The name is used by the doDraw routines.
                    // to get the correct asset, which uses a dictionary by name.
                    std::string name;
                    sf::Vector2f pos;
                    pos.x = (index % horizTileNum) * ARENAMAN_TILE_WIDTH;
                    pos.y = row * ARENAMAN_TILE_HEIGHT;
                    
                    arenaData[index].setPosition(pos);
                    switch (id){
                    case 0:
                        name = "Floor1";
                        break;
                    case 1:
                        name = "Floor2";
                        break;
                    case 2:
                        name = "Floor3";
                        break;
                    case 3:
                        name = "Team1Garage";
                        team1BasePos = pos;
                        team1BasePos.x += 62;
                        team1BasePos.y += 62;
                        break;
                    case 4:
                        name = "Team2Garage";
                        team2BasePos = pos;
                        team2BasePos.x += 62;
                        team2BasePos.y += 62;
                        break;
                    case 5:
                        name = "Team1Generator";
                        team1GenPos.push_back(pos);
                        break;
                    case 6:
                        name = "Team2Generator";
                        team2GenPos.push_back(pos);
                        break;
                    }
                    arenaData[index].setName(name);

                    index++;

                    if (index % horizTileNum == 0)
                        row++;

                }
            }else{
                std::cout << "ArenaManager::load unable to open map" << std::endl;
                return 1;
            }
            return 0;
        }

        bool indexToRC(int index, int & r, int & c)
        {
            sf::Vector2f pos;

            int row = index / horizTileNum;

            r = (index % horizTileNum) * ARENAMAN_TILE_WIDTH;
            c = row * ARENAMAN_TILE_HEIGHT;

            return true;
        }

        bool posToIndex(sf::Vector2f pos, int & index)
        {
            index = (pos.x / horizTileNum) * (pos.y / vertTileNum);
            return true;
        }

        sf::Uint32 setSprites(AssetManager & am, int screenWidth, int screenHeight);
        sf::Vector2f getStartPosition(int team){
            /*for (auto i = arenaData.begin();i != arenaData.end();i++){
                if (i->getId() == 3 && team == 1 ||
                    i->getId() == 4 && team == 2)
                {
                    sf::Vector2f p = i->getPosition();
                    if (team == 1){
                        p.x += 62;
                        p.y += 62;
                    }else if (team == 2){
                        p.x += 62;
                        p.y += 62;
                    }
                    return p;
                }
            }
            return sf::Vector2f(0,0);*/
            if (team == 1){
                return team1BasePos;
            }else if (team == 2){
                return team2BasePos;
            }
        }

        sf::Uint32 getGeneratorCount(int team){
             if (team == 1)
            {
                return team1GenPos.size();
            }else// if (team == 2)
            {
                return team2GenPos.size();
            }
        }

        sf::Vector2f getGeneratorPosition(int team, int gi){
            /*for (auto i = arenaData.begin();i != arenaData.end();i++){
                if (i->getId() == 5 && team == 1 ||
                    i->getId() == 6 && team == 2)
                {
                    sf::Vector2f p = i->getPosition();
                    if (team == 1){
                        p.x += 62;
                        p.y += 62;
                    }else if (team == 2){
                        p.x += 62;
                        p.y += 62;
                    }
                    return p;
                }
            }
            return sf::Vector2f(0,0);*/
            if (team == 1)
            {
                return team1GenPos[gi];
            }else// if (team == 2)
            {
                return team2GenPos[gi];
            }
        }

        sf::Uint32 getMapHorizTileNum(){
            return horizTileNum;
        }

        sf::Uint32 getMapVertTileNum(){
            return vertTileNum;
        }

        Tile & getTile(sf::Uint32 index){
            return arenaData[index];
        }
    
        sf::Uint32 horizTileNum;
        sf::Uint32 vertTileNum;
private:
        std::vector<Tile>  arenaData;
        std::vector<sf::Vector2f> team1GenPos;
        std::vector<sf::Vector2f> team2GenPos;
        sf::Vector2f team1BasePos;
        sf::Vector2f team2BasePos;
        
    };
}

#endif

