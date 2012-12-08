#include "ArenaManager.h"
#include "AssetManager.h"
#include "LogFile.h"
#include <fstream>
#include <iostream>
using namespace tg;

sf::Uint32 ArenaManager::setSprites(AssetManager & am, int screenWidth, int screenHeight)
{
    //bool ret = false;
    //sf::Uint32 width;
    //sf::Uint32 height;
    ////Read map file just for width and height.
    ////Every 4 bytes represents a distinct datum. 
    ////First datum is width
    ////Second datum is height
    ////Remaining data is tile type. There are width*height 4 byte tile type id following width and height
    //// each tile type can be:
    ////  0 - Wall
    ////  1 - Floor 1
    ////  2 - Floor 2
    ////  3 - Team 1 Garage
    ////  4 - Team 2 Garage
    ////  5 - Team 1 Minion Generator
    ////  6 - Team 2 Minion Generator
    //std::ifstream fin("Assets\\map_garage_mgenerator.txt",std::ios::in|std::ios::binary  );
    //if (fin.is_open()){
    //    fin.read((char*)&width, 4);
    //    fin.read((char*)&height, 4);
    //    //Populate sprite vector with default sprites which represents the entire map.
    //    //we set any ol texture here, just so we can get it's size.
    //    //TODO: if all tiles are same size, we can optimize and only do spriteWidth\Height once.

    //    for (int h = 0;h < height;h++){
    //        for (int w = 0; w < width;w++){
    //            FloorSprite ft;
    //            ft.sprite.setTexture(*am.floorImages.back().tex);
    //            floorSprites.push_back(ft);
    //            float spriteWidth = floorSprites.back().sprite.getGlobalBounds().width;
    //            float spriteHeight = floorSprites.back().sprite.getGlobalBounds().height;
    //            floorSprites.back().sprite.setPosition((float)(w*spriteWidth-spriteWidth/2),(float)(h*spriteHeight-spriteHeight/2));
    //        }
    //    }

    //    //Now read map file, and tailor the sprite vector to have the corret tile.
    //    int index = 0;

    //    while (!fin.eof()){
    //        int i;
    //        fin.read((char*)&i, 4);
    //        if (index == floorSprites.size())
    //            break;
    //        floorSprites[index].id = i;
    //        floorSprites[index].sprite.setTexture(*am.floorImages[i].tex);
    //        index++;
    //        //std::cout << index << std::endl;
    //                
    //    }
    //    LogFile::get()->log(0,0,"configured floor sprites");
    //}
    return 0;
}