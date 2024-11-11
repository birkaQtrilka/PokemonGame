#include <fstream>
#include <SFML/Audio/SoundBuffer.hpp>

#include "BattleScreen.h"
#include "Console.h"
#include "sceneHandler.hpp"
#include "character.hpp"
extern std::string folderPrefix;



BattleScreen::BattleScreen(std::string identifier, SceneHandler& manager) : Scene(identifier, manager,sf::Color(7,79,87))
{
    SetUpInterface();
    SetUpBehavior();
}

BattleScreen::~BattleScreen()
{

}

void BattleScreen::SetUpInterface()
{
    e_name = std::make_shared<TextObject>(folderPrefix + "characterNameText", manager->mainFont, "nothing");
    e_name->setPosition(sf::Vector2f(848.0f, 34.0f));
    e_name->setCharacterSize(26);
    e_name->setFillColor(manager->black);

    e_hpText = std::make_shared<TextObject>("phpText", manager->mainFont, "HP: -1");
    e_hpText->setPosition(sf::Vector2f(848.0f, 238.0f));
    e_hpText->setCharacterSize(26);
    e_hpText->setFillColor(manager->black);

    e_atckText = std::make_shared<TextObject>("attackText", manager->mainFont, "ATTACK: -1");
    e_atckText->setPosition(sf::Vector2f(848.0f, 290.0f));
    e_atckText->setCharacterSize(26);
    e_atckText->setFillColor(manager->black);

    e_dfnsText = std::make_shared<TextObject>("defenseText", manager->mainFont, "DEFENSE: -1");
    e_dfnsText->setPosition(sf::Vector2f(848.0f, 345.0f));
    e_dfnsText->setCharacterSize(26);
    e_dfnsText->setFillColor(manager->black);

    p_hpText = std::make_shared<TextObject>("hpText", manager->mainFont, "HP: -1");
    p_hpText->setPosition(sf::Vector2f(300, 300.0f));
    p_hpText->setCharacterSize(26);
    p_hpText->setFillColor(manager->black);

    p_expText = std::make_shared<TextObject>("expText", manager->mainFont, "Exp: -1");
    p_expText->setPosition(sf::Vector2f(300, 350.0f));
    p_expText->setCharacterSize(26);
    p_expText->setFillColor(manager->black);

    battleConsole = std::make_shared<Console>("Console", manager->mainFont, "select action",
        sf::Vector2f(600, 180.0f), manager->darkColor, 4);
    battleConsole->setPosition(sf::Vector2f(408.0f, 500.0f));
    battleConsole->setCharacterSize(20);

    backBtn = std::make_shared<Button>("BackBtn", manager->mainFont, "Back",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    backBtn->setPosition(sf::Vector2f(0, 0));

    quitBtn = std::make_shared<Button>("QuitBtn", manager->mainFont, "Quit",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    quitBtn->setPosition(sf::Vector2f(0, 55.0f));

    attackBtn = std::make_shared<Button>("AttackBtn", manager->mainFont, "Attack",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    attackBtn->setPosition(sf::Vector2f(58.0f, 500.0f));
             
    healBtn = std::make_shared<Button>("HealBtn", manager->mainFont, "Heal",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    healBtn->setPosition(sf::Vector2f(58.0f, 555.0f));

    doNthnBtn = std::make_shared<Button>("DoNthnBtn", manager->mainFont, "Do Nothing",
        sf::Vector2f(192.5f, 50.0f), manager->darkColor);
    doNthnBtn->setPosition(sf::Vector2f(58.0f, 610.0f));

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

void BattleScreen::SetUpBehavior()
{
    onEnter([&]()
    {
        if (!LoadBattleFile())
        {
            manager->changeToScene("StartScene");
            return;
        }
        RegisterData();
        //const reference. Do I need a non const reference?
        enemies.back()->UpdateText(e_name, e_hpText,e_atckText,e_dfnsText);
        enemies.back()->SetActive(true);
        
        UpdateTxt(p_hpText, "Your HP: ", loadedPlayer->getHP());
        UpdateTxt(p_expText, "Your Exp: ", loadedPlayer->getExp());
        battleConsole->clearText();
    });

    onExit([&]()
    {
        SaveData();
        if(loadedPlayer)
            removeGameObject(loadedPlayer);

        for (std::shared_ptr<Character> enemy : enemies)
        {
            if (enemy)
                removeGameObject(enemy);
        }
        enemies.clear();
    });


    attackBtn->setButtonAction([&]()
    {
        Character& enemy = *enemies.back();
    
        int damage = loadedPlayer->attackCharacter(enemy);
        
        battleConsole->pushText("You attacked with " + std::to_string(damage) + " damage!");
        sf::Sound& sound = s1;
        sound.setVolume(20);
        bool isKilled = enemy.takeDamage(damage);
        if (isKilled)
        {
            sound.setBuffer(*deathSoundBuffer);
            sound.play();
            int expGained = enemy.getExp();
            loadedPlayer->setExp(loadedPlayer->getExp() + expGained);
            UpdateTxt(p_expText, "Your Exp: ", loadedPlayer->getExp());

            KillEnemy();

            bool isLastEnemy = enemies.size() == 0;
            if (isLastEnemy)
            {
                RegisterData();

                manager->changeToScene("YouWinScene");

                return;
            }
            
            Character& newEnemy = *enemies.back();
            newEnemy.SetActive(true);
            newEnemy.UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);
            battleConsole->pushText("You defeated enemy! Exp gained: " + std::to_string(expGained));

            RegisterData();
            return;
        }
        sound.setBuffer(*attackSoundBuffer);
        sound.play();
        
        RegisterData();
        EnemyTurn();

        enemy.UpdateText(e_name,e_hpText,e_atckText,e_dfnsText);
        UpdateTxt(p_hpText, "Your HP:", loadedPlayer->getHP());

        //check if player dies
        if (loadedPlayer->getHP() != 0) return;
        
        manager->changeToScene("YouLostScene");
    
    });
    
    healBtn->setButtonAction([&]() 
    {
        Character& enemy = *enemies.back();
    
        int heal = loadedPlayer->HealSelf();
        battleConsole->pushText("You healed with " + std::to_string(heal) + " points!");
        
        sf::Sound& sound = s2;
        sound.setBuffer(*healSoundBuffer);
        sound.setVolume(30);
        sound.play();
        
        RegisterData();
        EnemyTurn();
        
        enemy.UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);
        
        UpdateTxt(p_hpText, "Your HP:", loadedPlayer->getHP());

        //check if player dies
        if (loadedPlayer->getHP() != 0) return;
    
        manager->changeToScene("YouLostScene");
    });
    
    doNthnBtn->setButtonAction([&]()
    {
        battleConsole->pushText("You did nothing..");

        RegisterData();
        EnemyTurn();

        enemies.back()->UpdateText(e_name, e_hpText, e_atckText, e_dfnsText);

        UpdateTxt(p_hpText, "Your HP:", loadedPlayer->getHP());
    });
    
    backBtn->setButtonAction([&]()
    {
        manager->changeToScene("StartScene");
    });

    quitBtn->setButtonAction([&]()
    {
        SaveData();
        manager->window->close();
    });

    
}

bool BattleScreen::LoadBattleFile()
{
    std::ifstream battleSaveFileRead(folderPrefix + "data.cmgt");
    std::string line;
    std::getline(battleSaveFileRead, line);

    if (battleSaveFileRead.fail()) {
        battleSaveFileRead.close();
        return false;
    }

    std::string playerLine = line;

    std::getline(battleSaveFileRead, line);
    int enemyCount = std::stoi(line);

    if (enemyCount == 0)
    {
        battleSaveFileRead.close();
        return false;
    }

    loadedPlayer = std::make_shared<Character>(playerLine);
    addGameObject(loadedPlayer);
    loadedPlayer->setPosition(sf::Vector2f(300, 120.0f));
    loadedPlayer->setScale(sf::Vector2f(6.0f, 6.0f));

    for (size_t i = 0; i < enemyCount; i++)
    {
        if (battleSaveFileRead.fail())
        {
            battleSaveFileRead.close();
            return false;
        }

        std::getline(battleSaveFileRead, line);

        std::shared_ptr<Character> currentEnemy = std::make_shared<Character>(line);
        enemies.push_back(currentEnemy);
        addGameObject(currentEnemy);
        currentEnemy->SetActive(false);

        currentEnemy->setPosition(sf::Vector2f(848.0f, 90.0f));
        currentEnemy->setScale(sf::Vector2f(4.0f, 4.0f));
    }
    battleSaveFileRead.close();
    return true;
}

void BattleScreen::RegisterData()
{
    Character& player = *loadedPlayer;
    battleData = player.getName() + ',' +
        player.getSpriteFile() + ',' +
        std::to_string(player.getHP()) + ',' +
        std::to_string(player.getAttack()) + ',' +
        std::to_string(player.getDefense()) + ',' +
        std::to_string(player.getExp())
        + '\n' +
        std::to_string(enemies.size()) + '\n';

    for (std::shared_ptr<Character> enemy : enemies)
    {
        battleData += enemy->getName() + ',' +
            enemy->getSpriteFile() + ',' +
            std::to_string(enemy->getHP()) + ',' +
            std::to_string(enemy->getAttack()) + ',' +
            std::to_string(enemy->getDefense()) + ',' +
            std::to_string(enemy->getExp())
            + '\n';
    }
}

void BattleScreen::SaveData() const
{
    printf("Saving battle");
    std::ofstream myfileWrite(folderPrefix + "data.cmgt", std::ios::trunc);
    
    myfileWrite << battleData;

    myfileWrite.close();
}


void BattleScreen::KillEnemy()
{
    removeGameObject(enemies.back());
    enemies.pop_back();
}

void BattleScreen::UpdateTxt(std::shared_ptr<TextObject> healthTxt, const std::string& start, const int val )
{
    healthTxt->setText(start + " " + std::to_string(val) );
}

void BattleScreen::EnemyTurn()
{
    //save progress here
    Character& enemy = *enemies.back();
    Character& player = *loadedPlayer;

    switch ((rand() % 3))
    {
    case 0:
    {
        int dmg = enemy.attackCharacter(player);
        player.takeDamage(dmg);
        battleConsole->pushText("Enemy attacked with " + std::to_string(dmg) + " damage!");
    }
        break;
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

