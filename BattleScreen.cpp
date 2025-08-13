#include <fstream>
#include <SFML/Audio/SoundBuffer.hpp>

#include "BattleScreen.h"
#include "Console.h"
#include "sceneHandler.hpp"
#include "character.hpp"
#include "EnemyRandomValues.h"
#include "DifficultyManager.h"
#include "SceneAssets.h"
#include <sstream>
#include "UIHelper.h"

extern std::string folderPrefix;

BattleScreen::BattleScreen(std::string identifier, SceneHandler& manager) : Scene(identifier, manager,sf::Color(7,79,87))
{
    SetUpInterface();
    SetUpBehavior();
    SetUpUpgradeWindow();
}

BattleScreen::~BattleScreen()
{

}

void BattleScreen::InitBattleFile(std::shared_ptr<Character> player, int difficulty)
{
    serializer.NewBattleFile(*player, difficulty);
}

void BattleScreen::SetUpInterface()
{
    SceneAssets* assets = SceneAssets::GetInstance();
    const sf::Vector2f btnSize(192.5f, 50.0f);
    sf::String e_name_id = folderPrefix + "characterNameText";
    int txtSize = assets->h2FontSize;
    e_name = UIHelper::CreateText(e_name_id, assets->mainFont,          "nothing", sf::Vector2f(848.0f, 34.0f), txtSize, assets->black);
    e_hpText = UIHelper::CreateText("phpText", assets->mainFont,        "HP: -1", sf::Vector2f(848.0f, 238.0f), txtSize, assets->black);
    e_atckText = UIHelper::CreateText("attackText", assets->mainFont,   "ATTACK: -1", sf::Vector2f(848.0f, 290.0f), txtSize, assets->black);
    e_dfnsText = UIHelper::CreateText("defenseText", assets->mainFont,  "DEFENSE: -1", sf::Vector2f(848.0f, 345.0f), txtSize, assets->black);

    p_hpText = UIHelper::CreateText("hpText", assets->mainFont,         "HP: -1", sf::Vector2f(300, 300.0f), txtSize, assets->black);
    p_expText = UIHelper::CreateText("expText", assets->mainFont,       "Exp: -1", sf::Vector2f(300, 350.0f), txtSize, assets->black);

    battleConsole = std::make_shared<Console>("Console", assets->mainFont, "select action",
        sf::Vector2f(600, 180.0f), assets->darkColor, 4);
    battleConsole->setPosition(sf::Vector2f(408.0f, 500.0f));
    battleConsole->setCharacterSize(20);

    backBtn = UIHelper::CreateButton("BackBtn", assets->mainFont,       "Back", btnSize, assets->darkColor, sf::Vector2f(0, 0));
    quitBtn = UIHelper::CreateButton("QuitBtn", assets->mainFont,       "Quit", btnSize, assets->darkColor, sf::Vector2f(0, 55.0f));
    attackBtn = UIHelper::CreateButton("AttackBtn", assets->mainFont,   "Attack", btnSize, assets->darkColor, sf::Vector2f(58.0f, 500.0f));
    healBtn = UIHelper::CreateButton("HealBtn", assets->mainFont,       "Heal", btnSize, assets->darkColor, sf::Vector2f(58.0f, 555.0f));
    doNthnBtn = UIHelper::CreateButton("DoNthnBtn", assets->mainFont,   "Do Nothing", btnSize, assets->darkColor, sf::Vector2f(58.0f, 610.0f));

    attackSoundBuffer = std::make_shared<sf::SoundBuffer>();
    attackSoundBuffer->loadFromFile(folderPrefix + "8-bit-punch-short.wav");

    healSoundBuffer = std::make_shared<sf::SoundBuffer>();
    healSoundBuffer->loadFromFile(folderPrefix + "powerUp.mp3");

    deathSoundBuffer = std::make_shared<sf::SoundBuffer>();
    deathSoundBuffer->loadFromFile(folderPrefix + "deathSound.mp3");

    addGameObject(e_name);
    addGameObject(e_hpText);
    addGameObject(e_atckText);
    addGameObject(e_dfnsText);
    addGameObject(battleConsole);
    addGameObject(p_hpText);
    addGameObject(p_expText);

    addGameObject(attackBtn);
    addGameObject(healBtn);
    addGameObject(backBtn);
    addGameObject(quitBtn);
    addGameObject(doNthnBtn);
}

void BattleScreen::SetUpUpgradeWindow()
{
    auto windowSize = manager->window->getSize();
    sf::Vector2f offset((float)windowSize.x / 2 - 250, 390);
    SceneAssets* assets = SceneAssets::GetInstance();
    const sf::Vector2f btnSize(200.5f, 50.0f);

    u_atckText = UIHelper::CreateText("AtckText", assets->mainFont, "your attack", offset, assets->h2FontSize, assets->black);
    u_dfnsText = UIHelper::CreateText("dfnsText", assets->mainFont, "your defense", offset + sf::Vector2f(220, 0), assets->h2FontSize, assets->black);

    offset.y += 55;
    u_atckBtn = UIHelper::CreateButton("AttackBtn", assets->mainFont, "Upgrade Attack", btnSize, assets->darkColor, offset);
    u_dfnsBtn = UIHelper::CreateButton("DefenseBtn", assets->mainFont, "Upgrade Defense", btnSize, assets->darkColor, offset + sf::Vector2f(220, 0));

    std::random_device rd;
    std::mt19937 gen(rd());

    u_atckBtn->setButtonAction([&]() {
        EnemyRandomValues values = DifficultyManager::GetRandomValues(state.difficulty);
        Character& player = *state.loadedPlayer;

        int addAmount = values.attack(gen);
        battleConsole->pushText("Added strength: " + std::to_string(addAmount));

        player.setAttack(player.getAttack() + addAmount);
        serializer.RegisterData(state);
        CloseUpgradeWindow();
        });

    u_dfnsBtn->setButtonAction([&]() {
        EnemyRandomValues values = DifficultyManager::GetRandomValues(state.difficulty);
        Character& player = *state.loadedPlayer;

        int addAmount = values.defense(gen);
        battleConsole->pushText("Added defense: " + std::to_string(addAmount));

        player.setDefense(player.getDefense() + addAmount);
        serializer.RegisterData(state);
        CloseUpgradeWindow();
        });

    addGameObject(u_atckBtn);
    addGameObject(u_dfnsBtn);
    addGameObject(u_atckText);
    addGameObject(u_dfnsText);

    CloseUpgradeWindow();
}

void BattleScreen::OpenUpgradeWindow()
{
    u_atckText->setText("your attack: " + std::to_string(state.loadedPlayer->getAttack()));
    u_dfnsText->setText("your defense: " + std::to_string(state.loadedPlayer->getDefense()));

    u_atckBtn->SetActive(true);
    u_atckText->SetActive(true);

    u_dfnsBtn->SetActive(true);
    u_dfnsText->SetActive(true);
    allowPress = false;
}

void BattleScreen::CloseUpgradeWindow()
{
    u_atckBtn->SetActive(false);
    u_atckText->SetActive(false);

    u_dfnsBtn->SetActive( false);
    u_dfnsText->SetActive(false);
    allowPress = true;

}

void BattleScreen::SetUpBehavior()
{
    onEnter([&]()
    {
        bool success;
        std::tie(success, state) = serializer.LoadBattleFile();
        if (!success)
        {
            manager->changeToScene("StartScene");
            return;
        }
        serializer.RegisterData(state);
        
        addGameObject(state.loadedPlayer);
        state.loadedPlayer->setPosition(sf::Vector2f(300, 120.0f));
        state.loadedPlayer->setScale(sf::Vector2f(6.0f, 6.0f));

        for (std::shared_ptr<Character> enemy : state.enemies)
        {
            addGameObject(enemy);
            enemy->SetActive(false);
            enemy->setPosition(sf::Vector2f(848.0f, 90.0f));
            enemy->setScale(sf::Vector2f(4.0f, 4.0f));
        }

        GetCurrentEnemy().UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);
        GetCurrentEnemy().SetActive(true);
        
        UpdateTxt(p_hpText, "Your HP: ", state.loadedPlayer->getHP());
        UpdateTxt(p_expText, "Your Exp: ", state.loadedPlayer->getExp());
        battleConsole->clearText();

    });

    onExit([&]()
    {
        serializer.SaveData();
        if(state.loadedPlayer) removeGameObject(state.loadedPlayer);

        for (std::shared_ptr<Character> enemy : state.enemies)
        {
            if (enemy) removeGameObject(enemy);
        }
        state.enemies.clear();
    });

    attackBtn->setButtonAction([&]()
    {
        if (!allowPress) return;
        OnAttackPress();
    
    });
    
    healBtn->setButtonAction([&]
    {
        if (!allowPress) return;
        OnHealPress();
    });
    
    doNthnBtn->setButtonAction([&]()
    {
        if (!allowPress) return;
        OnDoNothingPress();
    });
    
    backBtn->setButtonAction([&]()
    {
        if (!allowPress) return;

        manager->changeToScene("StartScene");
    });

    quitBtn->setButtonAction([&]()
    {
        if (!allowPress) return;

        manager->ShutDown();
    });

    
}

Character& BattleScreen::GetCurrentEnemy() const
{
    return *state.enemies[state.currentEnemyIndex];
}

void BattleScreen::IterateEnemy()
{
    state.currentEnemyIndex++;
    if (state.currentEnemyIndex < state.enemies.size()) return;
    state.currentEnemyIndex = 0;
    SetNewWaveOfEnemies();
}

void BattleScreen::SetNewWaveOfEnemies()
{
    auto clamp = [](int n, int lower, int upper)
    {
        return std::max(lower, std::min(n, upper));
    };
    std::random_device rd;
    std::mt19937 gen(rd());
    state.currentIteration++;
    EnemyRandomValues values = DifficultyManager::GetRandomValues(state.difficulty);

    for (std::shared_ptr<Character> enemy : state.enemies)
    {
        enemy->setHP(values.health(gen) * state.currentIteration);
        enemy->setAttack(enemy->getAttack() + clamp(values.attack(gen) / 2, 1, 5));
        enemy->setDefense(enemy->getDefense() + clamp(values.defense(gen) / 2, 1, 5));
        enemy->setExp(enemy->getExp() + clamp(values.exp(gen) / 2, 1, 5));
    }
}

void BattleScreen::KillEnemy()
{
    GetCurrentEnemy().SetActive(false);
}

void BattleScreen::UpdateTxt(std::shared_ptr<TextObject> healthTxt, const std::string& start, const int val )
{
    healthTxt->setText(start + " " + std::to_string(val) );
}

void BattleScreen::EnemyTurn()
{
    //save progress here
    Character& enemy = GetCurrentEnemy();
    Character& player = *state.loadedPlayer;

    switch ((rand() % 3))
    {
        case 0:
        {
            int dmg = enemy.attackCharacter(player);
            player.takeDamage(dmg);
            battleConsole->pushText("Enemy attacked with " + std::to_string(dmg) + " damage!");
            break;
        }
        case 1:
        {
            int heal = enemy.HealSelf();
            battleConsole->pushText("Enemy healed with " + std::to_string(heal) + " points!");
            break;
        }
        case 2:
            battleConsole->pushText("Enemy did nothing!");
            break;
    }
}

void BattleScreen::OnEnemyKill(sf::Sound& sound, Character& enemy)
{
    sound.setBuffer(*deathSoundBuffer);
    sound.play();
    int expGained = enemy.getExp();
    state.loadedPlayer->setExp(state.loadedPlayer->getExp() + expGained);
    UpdateTxt(p_expText, "Your Exp: ", state.loadedPlayer->getExp());

    KillEnemy();
    int prevIteration = state.currentIteration;
    IterateEnemy();

    Character& newEnemy = GetCurrentEnemy();
    newEnemy.SetActive(true);
    newEnemy.UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);
    battleConsole->pushText("You defeated enemy! Exp gained: " + std::to_string(expGained));

    serializer.RegisterData(state);

    if (state.currentIteration != prevIteration)
    {
        OpenUpgradeWindow();
    }
}

void BattleScreen::OnEnemyDamage(sf::Sound& sound, Character& enemy)
{
    sound.setBuffer(*attackSoundBuffer);
    sound.play();

    serializer.RegisterData(state);
    EnemyTurn();

    enemy.UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);
    UpdateTxt(p_hpText, "Your HP:", state.loadedPlayer->getHP());
}

void BattleScreen::OnAttackPress()
{
    Character& enemy = GetCurrentEnemy();

    int damage = state.loadedPlayer->attackCharacter(enemy);

    battleConsole->pushText("You attacked with " + std::to_string(damage) + " damage!");
    sf::Sound& sound = s1;
    sound.setVolume(20);
    bool isKilled = enemy.takeDamage(damage);

    if (isKilled)
    {
        OnEnemyKill(sound, enemy);
    }
    else
    {
        OnEnemyDamage(sound, enemy);
    }

    //check if player dies
    if (state.loadedPlayer->getHP() != 0) return;

    manager->changeToScene("YouWinScene");
}

void BattleScreen::OnHealPress()
{
    Character& enemy = GetCurrentEnemy();

    int heal = state.loadedPlayer->HealSelf();
    battleConsole->pushText("You healed with " + std::to_string(heal) + " points!");

    sf::Sound& sound = s2;
    sound.setBuffer(*healSoundBuffer);
    sound.setVolume(30);
    sound.play();

    serializer.RegisterData(state);
    EnemyTurn();

    enemy.UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);

    UpdateTxt(p_hpText, "Your HP:", state.loadedPlayer->getHP());

    //check if player dies
    if (state.loadedPlayer->getHP() != 0) return;

    manager->changeToScene("YouLostScene");
}

void BattleScreen::OnDoNothingPress()
{
    battleConsole->pushText("You did nothing..");

    serializer.RegisterData(state);
    EnemyTurn();

    state.enemies.back()->UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);

    UpdateTxt(p_hpText, "Your HP:", state.loadedPlayer->getHP());
}

