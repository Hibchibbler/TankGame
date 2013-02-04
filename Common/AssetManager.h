#ifndef AssetManager_h_
#define AssetManager_h_


#include "Images.h"
#include <map>
#include <vector>

namespace tg
{
    struct ImageType{
        enum siat{
            Projectile1,
            ProjectileShadow,
            ProjectileHealRay,
            ProjectileDeathRay,
            FloorWall1,
            Floor1,
            Floor2,
            FloorBase1,
            FloorBase2,
            FloorGenerator1,
            FloorGenerator2,
            FogOfWar,
            Minion1,
            Minion2,
            Dash1,
            ExplosionSmall,
            ExplosionBig,
            TankBlue,
            TankRed,
            TankShadow,
            TurretBlue,
            TurretRed,
            TurretShadow,
            MAX_IT
        };
    };

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

