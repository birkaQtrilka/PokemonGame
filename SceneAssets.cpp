#include "SceneAssets.h"
extern std::string folderPrefix;

SceneAssets* SceneAssets::_instance = nullptr;

SceneAssets::SceneAssets() : 
    enemyImages({ GetPath("plant.png"), GetPath("mushroom.png"),
        GetPath("amoeba.png"), GetPath("virus.png")})
{
    mainFont.loadFromFile(GetPath("Lato-Regular.ttf"));

}

SceneAssets* SceneAssets::GetInstance()
{
    if (!_instance) _instance = new SceneAssets();
    return _instance;
}

std::string SceneAssets::GetPath(const std::string& rootPath) const
{
    return folderPrefix + "Assets/" + rootPath;
}
