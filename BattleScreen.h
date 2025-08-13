#pragma once

#include "button.hpp"
#include "Console.h"
#include "character.hpp"
#include <stack>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "BattleState.h"
#include "BattleDataManager.h"


class BattleScreen : public Scene 
{
private:
    BattleState state;
    BattleDataManager serializer;

    std::shared_ptr<TextObject> e_name;
    std::shared_ptr<TextObject> e_hpText;
    std::shared_ptr<TextObject> p_hpText;
    std::shared_ptr<TextObject> p_expText;
    std::shared_ptr<TextObject> e_atckText;
    std::shared_ptr<TextObject> e_dfnsText;
    //upgrade window (u)
    std::shared_ptr<TextObject> u_atckText;
    std::shared_ptr<TextObject> u_dfnsText;
    std::shared_ptr<Button> u_atckBtn;
    std::shared_ptr<Button> u_dfnsBtn;

    std::shared_ptr<Button> backBtn;
    std::shared_ptr<Button> attackBtn;
    std::shared_ptr<Button> healBtn;
    std::shared_ptr<Button> doNthnBtn;
    std::shared_ptr<Button> quitBtn;

    std::shared_ptr<Console> battleConsole;
    std::shared_ptr<sf::SoundBuffer> attackSoundBuffer;
    std::shared_ptr<sf::SoundBuffer> healSoundBuffer;
    std::shared_ptr<sf::SoundBuffer> deathSoundBuffer;
    bool allowPress = true;

    sf::Sound s1;
    sf::Sound s2;
    sf::Sound s3;

public:
    BattleScreen(std::string identifier, SceneHandler& manager);
    ~BattleScreen();

    void InitBattleFile(std::shared_ptr<Character> player, int difficulty);

private:
    void SetUpInterface();
    void SetUpUpgradeWindow();
    void OpenUpgradeWindow();
    void CloseUpgradeWindow();

    void SetUpBehavior();
    void KillEnemy();
    void UpdateTxt(std::shared_ptr<TextObject> healthTxt, const std::string& start, const int val);
    Character& GetCurrentEnemy() const;
    void IterateEnemy();
    void SetNewWaveOfEnemies();

    void EnemyTurn();
    void OnEnemyKill(sf::Sound& sound, Character& enemy);
    void OnEnemyDamage(sf::Sound& sound, Character& enemy);
    void OnAttackPress();
    void OnHealPress();
    void OnDoNothingPress();
};

