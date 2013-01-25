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
            Minion1,
            Minion2,
            Dash1,
            Explosion1,
            Explosion2,
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
        //TankImage & getTankImage(int i);
        //Image & getImage(int i);

        sf::Sprite & getSprite(int i);
        
    private:
        sf::Font font;
        /*std::map<std::string, TankImage> tankImages;
        std::vector<FloorImage> floorImages;
        std::map<std::string, ProjectileImage> projectileImages;
        std::map<std::string, MinionImage> minionImages;
        Image tankDeathExplosionImage;
        Image tankHitExplosionImage;
        Image dashboardImage;*/


        //Image single_images[ImageType::MAX_IT];
        Image spriteSheet;
        Image smallExplosionSheet;
        Image bigExplosionSheet;
        //TankImage tank_images[TankImageType::MAX_TIT];
        
    };
};
#endif

