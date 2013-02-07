/**********************************************************************************
Tank Game
ArenaManager.h

Copyright 2012
Daniel Ferguson
**********************************************************************************/

#ifndef ArenaManager_h_
#define ArenaManager_h_

#include <SFML/Graphics.hpp>

#define ARENAMAN_TILE_WIDTH  128
#define ARENAMAN_TILE_HEIGHT 128

#include <iostream>
#include <fstream>
#include <vector>

namespace tg
{
    class Tile
    {
    public:
        Tile(){
            fog = true;
            visited = false;

        }
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

        //void setName(std::string n){name=n;}
        //std::string getName(){return name;}
    //private:
        sf::Vector2f position;
        sf::Uint32 id;
        bool fog;
        bool visited;
        //std::string name;
    };

    struct TileType{
        enum tt{
            Wall,
            Floor1,
            Floor2,
            Base1,
            Base2,
            Generator1,
            Generator2
        };
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

                //Each consecutive uint32 is a tile id
                int index = 0;
                int row = 0;
                arenaData.reserve(horizTileNum*vertTileNum);
                while (!fin.eof()){

                    sf::Uint32 id;
                    fin.read((char*)&id, 4);
                    arenaData.push_back(Tile());
                    arenaData[index].setId(id);

                    sf::Vector2f pos;
                    pos.x = (float)((index % horizTileNum) * ARENAMAN_TILE_WIDTH);
                    pos.y = (float)((index / horizTileNum) * ARENAMAN_TILE_HEIGHT);
                    
                    arenaData[index].setPosition(pos);
                    switch (id){
                    case TileType::Wall:
                        break;
                    case TileType::Floor1:
                        break;
                    case TileType::Floor2:
                        break;
                    case TileType::Base1:
                        team1BasePos = pos;
                        team1BasePos.x += 62;
                        team1BasePos.y += 62;
                        break;
                    case TileType::Base2:
                        team2BasePos = pos;
                        team2BasePos.x += 62;
                        team2BasePos.y += 62;
                        break;
                    case TileType::Generator1:
                        team1GenPos.push_back(pos);
                        break;
                    case TileType::Generator2:
                        team2GenPos.push_back(pos);
                        break;
                    }

                    index++;

                }
            }else{
                std::cout << "ArenaManager::load unable to open map" << std::endl;
                return 1;
            }
            return 0;
        }

        bool indexToPos(int index, int & r, int & c)
        {
            r = (index % horizTileNum) * ARENAMAN_TILE_WIDTH;
            c = (index / horizTileNum) * ARENAMAN_TILE_HEIGHT;
            return true;
        }

        void posToIndex(sf::Vector2f pos, int & index)
        {
            int w,h;
            w = ARENAMAN_TILE_WIDTH;
            h = ARENAMAN_TILE_HEIGHT;

            //quantize
            int gridCoordX = (((int)pos.x / w) * w);
            int gridCoordY = (((int)pos.y / h) * h);
            
            //normalize
            int mapCoordX = gridCoordX / w;
            int mapCoordY = gridCoordY / h;

            //convert
            index = mapCoordX + (mapCoordY*horizTileNum);
        }

        sf::Uint32 setSprites(AssetManager & am, int screenWidth, int screenHeight);
        sf::Vector2f getStartPosition(int team){
            if (team == 1){
                return team1BasePos;
            }else{// if (team == 2){
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
            if (team == 1)
            {
                return team1GenPos[gi];
            }else// if (team == 2)
            {
                return team2GenPos[gi];
            }
        }

        std::vector<sf::Vector2f> &  getGenerator(int team)
        {
            if (team == 1)
            {
                return team1GenPos;
            }else// if (team == 2)
            {
                return team2GenPos;
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

        std::vector<Tile>  arenaData;
private:
        std::vector<sf::Vector2f> team1GenPos;
        std::vector<sf::Vector2f> team2GenPos;
        sf::Vector2f team1BasePos;
        sf::Vector2f team2BasePos;
        
    };
}

#endif

