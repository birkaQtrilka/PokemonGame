#include "SceneAssets.h"
extern std::string folderPrefix;

SceneAssets* SceneAssets::_instance = nullptr;

SceneAssets::SceneAssets() : 
    enemyImages({ folderPrefix + "plant.png", folderPrefix + "mushroom.png",
        folderPrefix + "amoeba.png", folderPrefix + "virus.png" })
{
    mainFont.loadFromFile(folderPrefix + "Lato-Regular.ttf");

}

SceneAssets* SceneAssets::GetInstance()
{
    if (!_instance) _instance = new SceneAssets();
    return _instance;
}
