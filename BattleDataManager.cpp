#include "BattleDataManager.h"
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/System/Vector2.hpp>
#include "character.hpp"
#include <random>
#include "EnemyRandomValues.h"
#include "SceneAssets.h"
#include "DifficultyManager.h"

extern std::string folderPrefix;

BattleDataManager::BattleDataManager():
    dataPath(SceneAssets::GetInstance()->GetPath("data.cmgt"))
{
    
}

std::tuple<bool, BattleState> BattleDataManager::LoadBattleFile()
{
    std::ifstream battleSaveFileRead(dataPath);
    std::string line;
    std::getline(battleSaveFileRead, line);

    if (battleSaveFileRead.fail()) {
        battleSaveFileRead.close();
        return std::make_tuple(false, BattleState{});
    }
    BattleState state;

    std::string playerLine = line;


    std::getline(battleSaveFileRead, line);
    int enemyCount = std::stoi(line);

    std::getline(battleSaveFileRead, line);
    state.currentEnemyIndex = std::stoi(line);
    
    std::getline(battleSaveFileRead, line);
    state.difficulty = std::stoi(line);
    
    std::getline(battleSaveFileRead, line);
    state.currentIteration = std::stoi(line);

    if (enemyCount == 0)
    {
        battleSaveFileRead.close();
        return std::make_tuple(false, BattleState{});
    }
    state.loadedPlayer = std::make_shared<Character>(playerLine);

    for (size_t i = 0; i < enemyCount; i++)
    {
        if (battleSaveFileRead.fail())
        {
            battleSaveFileRead.close();
            return std::make_tuple(false, BattleState{});
        }

        std::getline(battleSaveFileRead, line);

        std::shared_ptr<Character> currentEnemy = std::make_shared<Character>(line);
        state.enemies.push_back(currentEnemy);
    }
    battleSaveFileRead.close();
    return std::make_tuple(true, state);;
}

void BattleDataManager::RegisterData(const BattleState state)
{
    const Character& player = *state.loadedPlayer;

    std::ostringstream stringStream;
    //player
    player.Serialize(stringStream);

    // Meta data
    stringStream << state.enemies.size() << '\n'
        << state.currentEnemyIndex << '\n'
        << state.difficulty << '\n'
        << state.currentIteration << '\n';
    //enemy
    for (auto& enemy : state.enemies)
    {
        enemy->Serialize(stringStream);
    }

    serializedData = stringStream.str();
}

void BattleDataManager::SaveData() const
{
    printf("Saving battle");
    std::ofstream myfileWrite(dataPath, std::ios::trunc);

    myfileWrite << serializedData;

    myfileWrite.close();
}

void BattleDataManager::NewBattleFile(const Character& player, const int difficulty) const 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    EnemyRandomValues r = DifficultyManager::GetRandomValues(difficulty);
    std::uniform_int_distribution<> randomSprite(0, SceneAssets::GetInstance()->enemyImages.size() - 1);

    int enemyCount = 5;

    std::ofstream myfileWrite(dataPath, std::ios::trunc);

    myfileWrite << player.getName() << ',' <<
        player.getSpriteFile() << ',' <<
        std::to_string(player.getHP()) << ',' <<
        std::to_string(player.getAttack()) << ',' <<
        std::to_string(player.getDefense()) << ',' <<
        std::to_string(player.getExp())
        << std::endl;
    myfileWrite << enemyCount << std::endl;
    myfileWrite << 0 << std::endl;
    myfileWrite << difficulty << std::endl;
    myfileWrite << 0 << std::endl;

    for (size_t i = 0; i < enemyCount; i++)
    {
        myfileWrite << "Enemy" + std::to_string(i) << ',' <<
            SceneAssets::GetInstance()->enemyImages[randomSprite(gen)] << ',' <<
            std::to_string(r.health(gen)) << ',' <<
            std::to_string(r.attack(gen)) << ',' <<
            std::to_string(r.defense(gen)) << ',' <<
            std::to_string(r.exp(gen))
            << std::endl;
    }
    myfileWrite.close();
}
