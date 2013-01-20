#ifndef AssetManager_h_
#define AssetManager_h_


#include "Images.h"
#include <map>

namespace tg
{
    class AssetManager
    {
    public:
        AssetManager();
        ~AssetManager();
        int load();

        sf::Font & getFont();
        TankImage & getTankImage(std::string name);
        FloorImage & getFloorImage(std::string name);
        ProjectileImage & getProjectileImage(std::string name);
        MinionImage & getMinionImage(std::string name);
        Image & getDashboardImage();
        Image & getTankDeathExplosionImage();
        Image & getTankHitExplosionImage();
    private:
        sf::Font font;
        std::map<std::string, TankImage> tankImages;
        std::map<std::string, FloorImage> floorImages;
        std::map<std::string, ProjectileImage> projectileImages;
        std::map<std::string, MinionImage> minionImages;
        Image tankDeathExplosionImage;
        Image tankHitExplosionImage;
        Image dashboardImage;
        
    };
};
#endif

