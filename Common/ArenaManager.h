/**********************************************************************************
Tank Game
ArenaManager.h

Copyright 2012
Daniel Ferguson
**********************************************************************************/

#ifndef ArenaManager_h_
#define ArenaManager_h_

#include <SFML/Graphics.hpp>
#include "Images.h"

#define ARENAMAN_TILE_WIDTH  128
#define ARENAMAN_TILE_HEIGHT 128

#include <iostream>
#include <fstream>
#include <vector>

namespace tg
{
    struct ElementType{
        enum e{
            Wall,
            Floor1,
            Floor2,
            Base,
            Turret,
            Waypoint,
            Totem
        };
    };

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
        sf::Uint32 getId(){
            return id;
        }
        void setPosition(sf::Vector2f pos){
            position = pos;
        }

        sf::Vector2f getPosition(){
            return position;
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

    class Entity
    {
    public:
        Entity(){

        }
        void setId(sf::Uint32 i){
            id = i;
        }
        sf::Uint32 getId(){
            return id;
        }
        sf::Vector2f position;
        sf::Uint32 id;
    };

    class BaseEntity : public Entity
    {
    };

    class TurretEntity : public Entity
    {
    };

    class WaypointEntity : public Entity
    {
    public:
        int type;
        int number;
    };

    class TotemEntity : public Entity
    {
    public:
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
                for (int r = 0;r < horizTileNum*vertTileNum;r++){

                    sf::Uint32 groundType;
                    fin.read((char*)&groundType, 4);//tileType is only used by the map Editor
                                                  //the game engine only uses attributes to identify.
                    arenaData.push_back(Tile());
                    sf::Uint32 attrNum=0;
                    sf::Uint32 elementType=-1;
                    fin.read((char*)&attrNum, 4);
                    for (int k = 0;k < attrNum;k++){
                        sf::Uint32 a;
                        fin.read((char*)&a, 4);
                        if (k==0)
                            elementType = a;
                        
                        arenaData.back().attributes.push_back(a);
                    }
                    if (elementType == -1){
                        std::cout << "ERROR: A tile in the map did not have an attribute" << std::endl;
                    }

                    int it = getImageType(elementType);
                    arenaData[index].setId(it);

                    sf::Vector2f pos;
                    pos.x = (float)((index % horizTileNum) * ARENAMAN_TILE_WIDTH);
                    pos.y = (float)((index / horizTileNum) * ARENAMAN_TILE_HEIGHT);
                    
                    arenaData[index].setPosition(pos);
                    
                    index++;


                    switch (it){
                    case ImageType::FloorWall1:
                        break;
                    case ImageType::Floor1:
                        break;
                    case ImageType::Floor2:
                        break;
                    }
                }

                int entityCount = 0;
                fin.read((char*)&entityCount,4);
                for (int e = 0;e < entityCount;e++)
                {
                    
                    sf::Uint32 entityType, attrCount;
                    sf::Uint32 x,y;
                    sf::Uint32 elementType=-1;
                    std::vector<int> attributes;

                    fin.read((char*)&entityType, 4);//Not used by game engine, only map editor.
                    fin.read((char*)&x, 4);
                    fin.read((char*)&y, 4);

                    sf::Vector2f entityPosition(x*128,y*128);

                    fin.read((char*)&attrCount,4);
                    for (int k = 0;k < attrCount;k++)
                    {
                        sf::Uint32 a;
                        fin.read((char*)&a, 4);
                        attributes.push_back(a);
                        if (k == 0)
                            elementType =a;
                    }
      
                    switch (elementType)
                    {
                    case ElementType::Base:{
                        BaseEntity be;
                        be.position = entityPosition;

                        if (attributes[1] == 1)
                        {
                            team1Base = be;
                        }else
                        {
                            team2Base = be;
                        }
                        break;
                                           }
                    case ElementType::Turret:{
                        TurretEntity te;
                        te.position = entityPosition;
                        te.position.x+=0;
                        te.position.y+=0;
                        if (attributes[1] == 1)
                        {
                            team1Turrets.push_back(te);
                        }else
                        {
                            team2Turrets.push_back(te);
                        }
                        break;        
                                             }
                    case ElementType::Waypoint:{
                        WaypointEntity we;
                        
                        we.type = attributes[2];
                        we.number = attributes[3];
                        we.position = entityPosition;
                        
                        if (attributes[1] == 1)
                            team1Waypoints.push_back(we);
                        else
                            team2Waypoints.push_back(we);
                        break;
                                               }
                    case ElementType::Totem:{
                        TotemEntity te;
                        te.position = entityPosition;

                        if (attributes[1] == 1)
                        {
                            team1Totems.push_back(te);
                        }else
                        {
                            team2Totems.push_back(te);
                        }
                        break;        
                                             }
                    
                    }

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
            case ElementType::Wall:
                return ImageType::FloorWall1;//Wall
            case ElementType::Floor1:
                return ImageType::Floor1;//Floor1
            case ElementType::Floor2:
                return ImageType::Floor2;//Floor2
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
                return team1Base.position;
            }else{// if (team == 2){
                return team2Base.position;
            }
        }

        sf::Uint32 getGeneratorCount(int team){
             if (team == 1)
            {
                return team1Turrets.size();
            }else// if (team == 2)
            {
                return team2Turrets.size();
            }
        }

        sf::Vector2f getGeneratorPosition(int team, int gi){
            if (team == 1)
            {
                return team1Turrets[gi].position;
            }else// if (team == 2)
            {
                return team2Turrets[gi].position;
            }
        }

        std::vector<TurretEntity> &  getGenerator(int team)
        {
            if (team == 1)
            {
                return team1Turrets;
            }else// if (team == 2)
            {
                return team2Turrets;
            }
        }

        std::vector<WaypointEntity> & getWaypoints(int team){
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
        /*sf::Vector2f team1BasePos;
        std::vector<sf::Vector2f> team1GenPos;*/
        BaseEntity                team1Base;
        std::vector<TurretEntity> team1Turrets;
        std::vector<WaypointEntity> team1Waypoints;
        std::vector<TotemEntity> team1Totems;

        BaseEntity                team2Base;
        std::vector<TurretEntity> team2Turrets;
        std::vector<WaypointEntity> team2Waypoints;
        std::vector<TotemEntity> team2Totems;

        
        
        
    };
}

#endif

