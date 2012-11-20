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
    private:
        sf::Vector2f position;
        sf::Uint32 id;
    };
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

                    

                    if (index % horizTileNum == (horizTileNum-1))
                        row++;

                    sf::Vector2f pos;
                    pos.x = (index % horizTileNum) * ARENAMAN_TILE_WIDTH;
                    pos.y = row * ARENAMAN_TILE_HEIGHT;

                    arenaData[index].setPosition(pos);
                    index++;
                    //std::cout << index << std::endl;
                }
            }
            return 0;
        }
        sf::Vector2f getStartPosition(int team){
            for (auto i = arenaData.begin();i != arenaData.end();i++){
                if (i->getId() == 3 && team == 1 ||
                    i->getId() == 4 && team == 2)
                {
                    return i->getPosition();
                }
            }
            return sf::Vector2f(0,0);
        }

        sf::Vector2f getGeneratorPosition(int team){
            for (auto i = arenaData.begin();i != arenaData.end();i++){
                if (i->getId() == 5 && team == 1 ||
                    i->getId() == 6 && team == 2)
                {
                    return i->getPosition();
                }
            }
            return sf::Vector2f(0,0);
        }
    private:
        sf::Uint32 horizTileNum;
        sf::Uint32 vertTileNum;
        std::vector<Tile>  arenaData;
        
    };
}

#endif

