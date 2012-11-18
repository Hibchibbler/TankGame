#ifndef AssetManager_h_
#define AssetManager_h_


#include "Images.h"
#include <map>

namespace tg
{
    class AssetManager
    {
    public:
        
        int load();
    private:
        std::map<std::string, TankImage> tankImages;
        std::map<std::string, FloorImage> floorImages;
        std::map<std::string, ProjectileImage> projectileImages;
        std::map<std::string, MinionImage> minionImages;
    };
};
#endif

