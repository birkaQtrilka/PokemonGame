#pragma once

#include "button.hpp"
#include "Console.h"
#include "character.hpp"
#include <stack>
#include <SFML/Audio/Sound.hpp>


class BattleScreen : public Scene
{
private:
    std::vector<std::shared_ptr<Character>> enemies;
    std::shared_ptr<Character> loadedPlayer;

    std::shared_ptr<TextObject> e_name;
    std::shared_ptr<TextObject> e_hpText;
    std::shared_ptr<TextObject> p_hpText;
    std::shared_ptr<TextObject> p_expText;
    std::shared_ptr<TextObject> e_atckText;
    std::shared_ptr<TextObject> e_dfnsText;
    std::shared_ptr<Button> backBtn;
    std::shared_ptr<Button> attackBtn;
    std::shared_ptr<Button> healBtn;
    std::shared_ptr<Button> doNthnBtn;
    std::shared_ptr<Button> quitBtn;

    std::shared_ptr<Console> battleConsole;
    std::shared_ptr<sf::SoundBuffer> attackSoundBuffer;
    std::shared_ptr<sf::SoundBuffer> healSoundBuffer;
    std::shared_ptr<sf::SoundBuffer> deathSoundBuffer;
    int currentEnemyIndex;
    int difficulty;
    int currentIteration;

    std::string battleData;
    sf::Sound s1;
    sf::Sound s2;
    sf::Sound s3;

public:
    BattleScreen(std::string identifier, SceneHandler& manager);
    ~BattleScreen();

private:
    void SetUpInterface();
    void SetUpBehavior();
    void RegisterData();
    void SaveData() const;
    void KillEnemy();
    void UpdateTxt(std::shared_ptr<TextObject> healthTxt, const std::string& start, const int val);
    bool LoadBattleFile();
    Character& GetCurrentEnemy() const;
    void IterateEnemy();
    void SetNewWaveOfEnemies();

    void EnemyTurn();


};

