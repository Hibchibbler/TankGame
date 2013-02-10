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
            fogTrueIndex = 5;
            fogFalseIndex=0;
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

       


    //private:
        sf::Vector2f position;
        sf::Uint32 id;
        bool fog;
        sf::Uint32 fogTrueIndex;
        sf::Uint32 fogFalseIndex;
        sf::Clock fogClock;
        std::vector<int> attributes;

    };

    struct TileType{
        enum tt{
            Wall,
            Floor1,
            Floor2,
            Base1,
            Base2,
            Generator1,
            Generator2,
            Waypoint1_1,
            Waypoint2_1,
            Waypoint3_1,
            Waypoint1_2,
            Waypoint2_2,
            Waypoint3_2
        };
    };

    class Waypoint
    {
    public:
        int wpType;
        int wpId;
        sf::Vector2f pos;
        std::vector<int> attributes;
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

                    sf::Uint32 tileType;
                    fin.read((char*)&tileType, 4);
                    arenaData.push_back(Tile());
                    int attrNum=0;
                    fin.read((char*)&attrNum, 4);
                    for (int k = 0;k < attrNum;k++){
                        int a;
                        fin.read((char*)&a, 4);
                        arenaData.back().attributes.push_back(a);
                    }

                    arenaData[index].setId(getImageType(tileType));

                    sf::Vector2f pos;
                    pos.x = (float)((index % horizTileNum) * ARENAMAN_TILE_WIDTH);
                    pos.y = (float)((index / horizTileNum) * ARENAMAN_TILE_HEIGHT);
                    
                    arenaData[index].setPosition(pos);
                    switch (tileType){
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
                    case TileType::Waypoint1_1:{
                        Waypoint wp;
                        wp.wpId = arenaData.back().attributes[0];
                        wp.wpType = 1;
                        wp.pos = pos;
                        
                        team1Waypoints.push_back(wp);
                        break;
                                               }
                    case TileType::Waypoint1_2:{
                        Waypoint wp;
                        wp.wpId = arenaData.back().attributes[0];
                        wp.wpType = 1;
                        wp.pos = pos;
                        team2Waypoints.push_back(wp);
                        break;
                                               }
                    case TileType::Waypoint2_1:{
                        Waypoint wp;
                        wp.wpId = arenaData.back().attributes[0];
                        wp.wpType = 2;
                        wp.pos = pos;
                        team1Waypoints.push_back(wp);
                        break;
                                               }
                    case TileType::Waypoint2_2:{
                        Waypoint wp;
                        wp.wpId = arenaData.back().attributes[0];
                        wp.wpType = 2;
                        wp.pos = pos;
                        team2Waypoints.push_back(wp);
                        break;
                                               }
                    }
                    
                    
                    index++;

                }
            }else{
                std::cout << "ArenaManager::load unable to open map" << std::endl;
                return 1;
            }
            return 0;
        }
         sf::Uint32 getImageType(sf::Uint32 tt){
            //This maps the tile id to the imagetype..
            //in StageRun.cpp => sf::Sprite s = g.assetMan.getSprite(tile.getId());
            switch (tt){
            case 0:
                return 4;
            case 1:
                return 5;
            case 2:
                return 6;
            case 3:
                return 7;
            case 4:
                return 8;
            case 5:
                return 9;
            case 6:
                return 10;
            case 7:
                return 5;
            case 8:
                return 5;
            case 9:
                return 5;
            case 10:
                return 5;
            case 11:
                return 5;
            case 12:
                return 5;
            }
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

        std::vector<Waypoint> & getWaypoints(int team){
            if (team == 1)
            {
                return team1Waypoints;
            }else// if (team == 2)
            {
                return team2Waypoints;
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

        std::vector<Waypoint> team1Waypoints;
        std::vector<Waypoint> team2Waypoints;

        sf::Vector2f team1BasePos;
        sf::Vector2f team2BasePos;
        
    };
}

#endif

