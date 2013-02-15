#ifndef AssetManager_h_
#define AssetManager_h_


#include "Images.h"
#include <map>
#include <vector>

namespace tg
{
    class AssetManager
    {
    public:
        AssetManager();
        ~AssetManager();
        int load();

        sf::Font & getFont();
        sf::Sprite & getSprite(int i);
        sf::Texture & getTexture(int i);
    private:
        sf::Font font;
        Image spriteSheet;
        Image smallExplosionSheet;
        Image bigExplosionSheet;
    };
};
#endif

