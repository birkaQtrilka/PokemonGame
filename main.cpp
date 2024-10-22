#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

#include "scene.hpp"
#include "textObject.hpp"
#include "spriteObject.hpp"
#include "character.hpp"
#include "button.hpp"
#include "sceneHandler.hpp"
#include <random>
#include "Console.h"
// I should add texture render reference to character

const int HighScoreListSize = 5;

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void GenerateBattleFile(std::mt19937& gen, std::vector<SpriteObject*>& enemySprites, std::vector<SpriteObject*>& playerSprites, Character& player)
{
    std::uniform_int_distribution<> hpRandom(10, 17);
    std::uniform_int_distribution<> attackRandom(1, 5);
    std::uniform_int_distribution<> defenseRandom(1, 3);
    std::uniform_int_distribution<> radnomEnemySprite(0, enemySprites.size()-1);
    std::uniform_int_distribution<> r_enemyCount(1, 4);
    std::uniform_int_distribution<> r_enemyExp(10, 20);
    
    std::ofstream myfileWrite("BattleSave.txt", std::ios::trunc);
    int enemyCount = r_enemyCount(gen);
    
    myfileWrite << player.getName() + ',' +
        playerSprites[player.getSpriteID()]->getSpriteFile() + ',' +
        std::to_string(player.getHP()) + ',' +
        std::to_string(player.getAttack()) + ',' +
        std::to_string(player.getDefense()) + ',' +
        std::to_string(player.getExp())
        << std::endl;

    myfileWrite << enemyCount << std::endl;

    for (size_t i = 0; i < enemyCount; i++)
    {
        myfileWrite << "Enemy" + std::to_string(i) + ',' +
            enemySprites[radnomEnemySprite(gen)]->getSpriteFile() + ',' +
            std::to_string(hpRandom(gen)) + ',' + 
            std::to_string(attackRandom(gen)) + ',' + 
            std::to_string(defenseRandom(gen)) + ',' +
            std::to_string(r_enemyExp(gen))
            << std::endl;
    }
    myfileWrite.close();
}

void SetCharacter(Character& character, std::string& line, std::vector<SpriteObject*>& sprites, Scene& scene)
{
    std::vector<std::string> strings = split(line, ',');//do I need to call strings.clear() at the end?

    character.setName(strings[0]);

    character.setSpriteID(sprites.size());
    SpriteObject* sprite = new SpriteObject(strings[1], strings[1]);
    sprite->SetActive(false);

    sprites.push_back(sprite);
    scene.addGameObject(*sprite);

    character.setHP(std::stoi(strings[2]));
    character.setAttack(std::stoi(strings[3]));
    character.setDefense(std::stoi(strings[4]));
    character.setExp(std::stoi(strings[5]));
    
    strings.clear();
}

void UpdateCharacterTxt(Character& previousCharacter,
    TextObject& nameTxt, std::vector<SpriteObject*>& sprites, TextObject& hpTxt,
    TextObject& attackTxt, TextObject& deffenseTxt, int& currentIndex,
    std::vector<Character*>& characters, Scene& scene)
{
    Character& currentCharacter = *characters[currentIndex];
    nameTxt.setText(currentCharacter.getName());

    sprites[previousCharacter.getSpriteID()]->SetActive(false);
    sprites[currentCharacter.getSpriteID()]->SetActive(true);

    attackTxt.setText(std::to_string(currentCharacter.getAttack()));
    hpTxt.setText(std::to_string(currentCharacter.getHP()));
    deffenseTxt.setText(std::to_string(currentCharacter.getDefense()));
}

void UpdateCharacterTxt(TextObject& nameTxt, std::vector<SpriteObject*>& sprites, TextObject& hpTxt,
    TextObject& attackTxt, TextObject& deffenseTxt, int& currentIndex,
    std::vector<Character*>& characters, Scene& scene)
{
    UpdateCharacterTxt(*characters[currentIndex], nameTxt, sprites, hpTxt, attackTxt, deffenseTxt, currentIndex, characters, scene);
}

void EnemyTurn(Character& player, Character& enemy, Console& console)
{
    switch ((rand() % 3))
    {
    case 0:
    {
        int dmg = enemy.attackCharacter(player);
        player.setHP(player.getHP() - dmg );
        console.pushText("Enemy attacked with " + std::to_string(dmg) + " damage!");
    }
        break;
    case 1:
    {
        int prevHp = enemy.getHP();
        enemy.HealSelf();
        int heal = enemy.getHP() - prevHp;
        console.pushText("Enemy healed with " + std::to_string(heal) + " points!");
        break;
    }
    case 2:
        console.pushText("Enemy did nothing!");
        break;
    }
}

void SaveProgress(std::vector<SpriteObject*>& enemySprites, std::vector<Character*>& enemies, Character& player, SpriteObject& playerSprite)
{
    std::ofstream myfileWrite("BattleSave.txt", std::ios::trunc);

    myfileWrite << player.getName() + ',' +
        playerSprite.getSpriteFile() + ',' +
        std::to_string(player.getHP()) + ',' +
        std::to_string(player.getAttack()) + ',' +
        std::to_string(player.getDefense()) + ',' +
        std::to_string(player.getExp())
        << std::endl;

    myfileWrite << enemies.size() << std::endl;

    for (size_t i = 0; i < enemies.size(); i++)
    {
        myfileWrite << enemies[i]->getName() + ',' +
            enemySprites[i]->getSpriteFile() + ',' +
            std::to_string(enemies[i]->getHP()) + ',' +
            std::to_string(enemies[i]->getAttack()) + ',' +
            std::to_string(enemies[i]->getDefense()) + ',' +
            std::to_string(enemies[i]->getExp()) 
            << std::endl;
    }

    myfileWrite.close();
}

void BubbleSort(std::vector<int>& arr) {
    bool swapped;
    int size = arr.size();

    for (int i = 0; i < size - 1; i++)
    {
        swapped = false;
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] < arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) break;
    }
}

std::vector<int> GetHighScores()
{
    std::ifstream myfileRead("HighScores.txt");
    std::string line;
    std::vector<int> highScores;

    int scoreAmount = 0;

    while (std::getline(myfileRead, line) && scoreAmount <= HighScoreListSize)
    {
        int score = std::stoi(line);
        highScores.push_back(score);
        scoreAmount++;
    }
    myfileRead.close();
    return highScores;
}

void DrawHighScore(TextObject& textMesh)
{
    std::string text;
    std::vector<int> highScores = GetHighScores();

    for (size_t i = 0; i < highScores.size(); i++)
    {
        text += std::to_string(i + 1) + '.' + std::to_string(highScores[i]) + '\n';
    }

    textMesh.setText(text);
    highScores.clear();
}

void SaveHighScore(const int highScore)
{
    std::vector<int> highScores = GetHighScores();
    int scoreAmount = highScores.size();

    if (scoreAmount < HighScoreListSize)
    {
        highScores.push_back(highScore);
        scoreAmount++;
    }
    else if (highScore > highScores[HighScoreListSize -1])
    {
        highScores[HighScoreListSize - 1] = highScore;
    }

    BubbleSort(highScores);
    
    std::ofstream myfileWrite("HighScores.txt", std::ios::trunc);

    for (size_t i = 0; i < scoreAmount; i++)
    {
        myfileWrite << std::to_string( highScores[i] )<< std::endl;
    }
    myfileWrite.close();
    highScores.clear();
}

void UpdateHealthTxt(TextObject& textMesh, const Character& character, const std::string& name)
{
    textMesh.setText(name + " health: " + std::to_string(character.getHP()));
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML with Scenes!");
    std::random_device rd;
    std::mt19937 gen(rd());

    Scene startScreen("StartScreen");
    Scene characterScreen("CharacterSelectionScene");
    Scene battleScene("BattleScene");
    Scene youLostScene("GameOverScene");
    Scene youWinScene("YouWinScene");
    
    SceneHandler sceneHandler(startScreen);

    std::vector<SpriteObject*> enemySprites;
    enemySprites.push_back(new SpriteObject("enemy1","enemy1.png"));
    enemySprites.push_back(new SpriteObject("enemy2","enemy2.png"));
    std::uniform_int_distribution<> r_enemyAction(0,2);


    std::vector<Character*> characters;
    std::vector<SpriteObject*> characterSprites;
    std::vector<Character*> enemies;
    int currentCharacterIndex = 0;
    int currentEnemyIndex = 0;


    sf::Font font;
    font.loadFromFile("Lato-Regular.ttf");
    sf::Color darkColor = sf::Color(71, 82, 94, 255);
    sf::Color darkGreyColor = sf::Color(132, 146, 166, 255);
    sf::Color lightGreyColor = sf::Color(129, 144, 165, 255);
    sf::Color programmerColor = sf::Color(200, 0, 0, 255);

    auto loadBattleFile = [&enemies, &enemySprites, &characters, &currentCharacterIndex, &characterSprites, &battleScene]()
        {
            std::ifstream battleSaveFileRead("BattleSave.txt");
            std::string line;
            std::getline(battleSaveFileRead, line);

            if (battleSaveFileRead.fail()) return;
            
            Character* loadedPlayer = new Character();
            characters.push_back(loadedPlayer);
            
            currentCharacterIndex = characters.size() - 1;

            SetCharacter(*loadedPlayer, line, characterSprites, battleScene);

            std::getline(battleSaveFileRead, line);
            int enemyCount = std::stoi(line);
            for (size_t i = 0; i < enemyCount; i++)
            {
                if (battleSaveFileRead.fail()) return;

                std::getline(battleSaveFileRead, line);
                
                Character* currentenemy = new Character();
                enemies.push_back(currentenemy);

                SetCharacter(*currentenemy, line, enemySprites, battleScene);
            }
            battleSaveFileRead.close();

            for (SpriteObject* enemySprite : enemySprites)
            {
                enemySprite->setPosition(sf::Vector2f(135.0f, 90.0f));
                enemySprite->setScale(sf::Vector2f(2.5f, 2.5f));
            }
            SpriteObject& characterSprite = *characterSprites[loadedPlayer->getSpriteID()];

            characterSprite.setPosition(sf::Vector2f(735.0f, 90.0f));
            characterSprite.setScale(sf::Vector2f(2.5f, 2.5f));
        };
    

#pragma region StartScreenSetup
    TextObject gameTitle("GameNameText", font, "PogaemonCo");
    gameTitle.setPosition(sf::Vector2f(109.0f, 34.0f));
    gameTitle.setCharacterSize(26);
    gameTitle.setFillColor(programmerColor);

    Button startNewBtn("attackItselfButton", font, "Start new",
        sf::Vector2f(192.5f, 50.0f), darkColor);
    startNewBtn.setPosition(sf::Vector2f(108.0f, 400.0f));

    Button continueBtn("attackItselfButton", font, "Continue",
        sf::Vector2f(192.5f, 50.0f), darkColor);
    continueBtn.setPosition(sf::Vector2f(108.0f, 500.0f));

    Button quitBtn("QuitBtn", font, "Quit",
        sf::Vector2f(192.5f, 50.0f), darkColor);
    quitBtn.setPosition(sf::Vector2f(108.0f, 600.0f));

    startScreen.addGameObject(gameTitle);
    startScreen.addGameObject(continueBtn);
    startScreen.addGameObject(startNewBtn);
    startScreen.addGameObject(quitBtn);
#pragma endregion

#pragma region CharacterSelectSetup
    TextObject characterName("characterNameText", font, "nothing");
    characterName.setPosition(sf::Vector2f(109.0f, 34.0f));
    characterName.setCharacterSize(26);
    characterName.setFillColor(darkColor);

    TextObject hpText("hpText", font, "HP: -1");
    hpText.setPosition(sf::Vector2f(161.0f, 238.0f));
    hpText.setCharacterSize(26);
    hpText.setFillColor(darkColor);

    TextObject attackText("attackText", font, "ATTACK: -1");
    attackText.setPosition(sf::Vector2f(148.0f, 290.0f));
    attackText.setCharacterSize(26);
    attackText.setFillColor(darkColor);

    TextObject defenseText("defenseText", font, "DEFENSE: -1");
    defenseText.setPosition(sf::Vector2f(140.0f, 345.0f));
    defenseText.setCharacterSize(26);
    defenseText.setFillColor(darkColor);

    Button charLeftBtn("CharLeftBtn", font, "<", sf::Vector2f(192.5f, 50.0f), darkColor);
    charLeftBtn.setPosition(sf::Vector2f(50.0f, 440.0f));

    Button charRightBtn("CharLeftBtn", font, ">", sf::Vector2f(192.5f, 50.0f), darkColor);
    charRightBtn.setPosition(sf::Vector2f(250.0f, 440.0f));

    Button quitButton("quitButton", font, "QUIT", sf::Vector2f(192.5f, 50.0f), darkColor);
    quitButton.setPosition(sf::Vector2f(108.0f, 540.0f));

    Button playBtn("quitButton", font, "Play", sf::Vector2f(192.5f, 50.0f), darkColor);
    playBtn.setPosition(sf::Vector2f(108.0f, 640.0f));

    characterScreen.addGameObject(characterName);

    characterScreen.addGameObject(hpText);
    characterScreen.addGameObject(attackText);
    characterScreen.addGameObject(defenseText);
    characterScreen.addGameObject(quitButton);
    characterScreen.addGameObject(charLeftBtn);
    characterScreen.addGameObject(charRightBtn);
    characterScreen.addGameObject(playBtn);
#pragma endregion

#pragma region BattleSceneSetup
    TextObject enemyName("characterNameText", font, "nothing");
    enemyName.setPosition(sf::Vector2f(409.0f, 34.0f));
    enemyName.setCharacterSize(26);
    enemyName.setFillColor(darkColor);

    TextObject enemyHpTxt("hpText", font, "HP: -1");
    enemyHpTxt.setPosition(sf::Vector2f(461.0f, 238.0f));
    enemyHpTxt.setCharacterSize(26);
    enemyHpTxt.setFillColor(darkColor);

    TextObject enemyAttackTxt("attackText", font, "ATTACK: -1");
    enemyAttackTxt.setPosition(sf::Vector2f(448.0f, 290.0f));
    enemyAttackTxt.setCharacterSize(26);
    enemyAttackTxt.setFillColor(darkColor);

    TextObject enemyDeffenseTxt("defenseText", font, "DEFENSE: -1");
    enemyDeffenseTxt.setPosition(sf::Vector2f(440.0f, 345.0f));
    enemyDeffenseTxt.setCharacterSize(26);
    enemyDeffenseTxt.setFillColor(darkColor);

    TextObject playerHealthTxt("hpText", font, "HP: -1");
    playerHealthTxt.setPosition(sf::Vector2f(0, 650.0f));
    playerHealthTxt.setCharacterSize(26);
    playerHealthTxt.setFillColor(darkColor);

    Console battleConsole("Console", font, "select action",
        sf::Vector2f(500, 150.0f), darkColor, 3);
    battleConsole.setPosition(sf::Vector2f(708.0f, 550.0f));
    battleConsole.setCharacterSize(12);

    Button attackBtn("AttackBtn", font, "Attack",
        sf::Vector2f(192.5f, 50.0f), darkColor);
    attackBtn.setPosition(sf::Vector2f(508.0f, 500.0f));

    Button healBtn("HealBtn", font, "Heal",
        sf::Vector2f(192.5f, 50.0f), darkColor);
    attackBtn.setPosition(sf::Vector2f(508.0f, 600.0f));

    Button quitFightBtn("attackItselfButton", font, "Quit",
        sf::Vector2f(192.5f, 50.0f), darkColor);
    quitFightBtn.setPosition(sf::Vector2f(708.0f, 500.0f));

    battleScene.addGameObject(enemyName);
    battleScene.addGameObject(enemyHpTxt);
    battleScene.addGameObject(enemyAttackTxt);
    battleScene.addGameObject(enemyDeffenseTxt);
    battleScene.addGameObject(attackBtn);
    battleScene.addGameObject(healBtn);
    battleScene.addGameObject(quitFightBtn);
    battleScene.addGameObject(battleConsole);
    battleScene.addGameObject(playerHealthTxt);
#pragma endregion

#pragma region You Won SetUp
    TextObject youWonText("YouWonText", font, "You Won!");
    youWonText.setPosition(sf::Vector2f(509.0f, 34.0f));
    youWonText.setCharacterSize(26);
    youWonText.setFillColor(programmerColor);

    TextObject highscoreText("YouWonText", font, "You Won!");
    highscoreText.setPosition(sf::Vector2f(509.0f, 100.0f));
    highscoreText.setCharacterSize(26);
    highscoreText.setFillColor(programmerColor);

    Button youWonQuitBtn("QuitBtn", font, "Quit",
        sf::Vector2f(509.5f, 50.0f), darkColor);
    youWonQuitBtn.setPosition(sf::Vector2f(108.0f, 600.0f));

    youWinScene.addGameObject(youWonText);
    youWinScene.addGameObject(highscoreText);
    youWinScene.addGameObject(youWonQuitBtn);
#pragma endregion

#pragma region You Lost SetUp
    TextObject youLostText("YouWonText", font, "You Lost!");
    youLostText.setPosition(sf::Vector2f(509.0f, 34.0f));
    youLostText.setCharacterSize(26);
    youLostText.setFillColor(programmerColor);

    Button youLostQuitBtn("QuitBtn", font, "Quit",
        sf::Vector2f(509.5f, 50.0f), darkColor);
    youLostQuitBtn.setPosition(sf::Vector2f(108.0f, 600.0f));

    youLostScene.addGameObject(youLostText);
    youLostScene.addGameObject(youLostQuitBtn);
#pragma endregion

#pragma region StartScreen

    startNewBtn.setButtonAction([&]()
        {
            sceneHandler.changeToScene(characterScreen);

            UpdateCharacterTxt(*characters[currentCharacterIndex], characterName,
                characterSprites, hpText, attackText, defenseText
                , currentCharacterIndex, characters, characterScreen);
        });

    continueBtn.setButtonAction([&]()
    {
            sceneHandler.changeToScene(battleScene);
    });
    

    quitBtn.setButtonAction([&window]()
        {
            window.close();
        });

    


#pragma endregion

#pragma region Character Screen
    std::ifstream characterSelectionFileRead("CharacterSelection.txt");
    std::string data;
    std::getline(characterSelectionFileRead, data);
    int characterCount = std::stoi(data);

    //Character character("Dude", "head.png", 10, 2, 2);
    for (size_t i = 0; i < characterCount; i++)
    {
        if (characterSelectionFileRead.fail()) return 0;
        
        std::getline(characterSelectionFileRead, data);
        
        Character* currentCharacter = new Character();
        characters.push_back(currentCharacter);

        SetCharacter(*currentCharacter, data, characterSprites, characterScreen);
    }
    characterSelectionFileRead.close();
    
    for (SpriteObject* characterAvatar : characterSprites)
    {
        characterAvatar->setPosition(sf::Vector2f(135.0f, 90.0f));
        characterAvatar->setScale(sf::Vector2f(2.5f, 2.5f));
    }

    charLeftBtn.setButtonAction([&]() {

        int previousIndex = currentCharacterIndex;
        if (currentCharacterIndex != 0)
            currentCharacterIndex--;
        UpdateCharacterTxt(*characters[previousIndex], characterName,
            characterSprites,hpText,attackText,defenseText
            ,currentCharacterIndex,characters,characterScreen);

    });


    charRightBtn.setButtonAction([&]() {
        int previousIndex = currentCharacterIndex;
        if (currentCharacterIndex != characterCount-1)
            currentCharacterIndex++;
        UpdateCharacterTxt(*characters[previousIndex], characterName,
            characterSprites, hpText, attackText, defenseText
            , currentCharacterIndex, characters, characterScreen);
    });

    quitButton.setButtonAction([&window] () {
        window.close();
    });
       

    playBtn.setButtonAction([&]()
    {
            GenerateBattleFile(gen, enemySprites, characterSprites, *characters[currentCharacterIndex]);
            

            sceneHandler.changeToScene(battleScene);

    });

#pragma endregion


#pragma region BattleScene
    //at window close, save enemies state 
    //show first enemy 

    battleScene.onEnter([&]()
        {
            loadBattleFile();

            UpdateCharacterTxt(enemyName, enemySprites, enemyHpTxt,
                enemyAttackTxt, enemyDeffenseTxt, currentEnemyIndex, enemies, battleScene);

            /*UpdateCharacterTxt(characterName,
                characterSprites, hpText, attackText, defenseText
                , currentCharacterIndex, characters, battleScene);*/
            UpdateHealthTxt(playerHealthTxt, *characters[currentCharacterIndex], "Your");
        });

    attackBtn.setButtonAction([&]()
        {
            Character& enemy = *enemies[currentEnemyIndex];
            Character& player = *characters[currentCharacterIndex];

            int damage = player.attackCharacter(enemy);
            enemy.setHP(enemy.getHP() - damage);
            
            battleConsole.pushText("You attacked with " + std::to_string(damage) + " damage!");

            //check if enemy character dies
            if (enemy.getHP() == 0)
            {
                if (currentEnemyIndex + 1 >= enemies.size())//last in array
                {
                    sceneHandler.changeToScene(youWinScene);
                    return;
                }
                player.setExp(player.getExp() + enemy.getExp());
                
                Character& newEnemy = *enemies[++currentEnemyIndex];
                UpdateCharacterTxt(enemy/*previous*/, enemyName, enemySprites, enemyHpTxt, enemyAttackTxt, enemyDeffenseTxt, currentEnemyIndex, enemies, battleScene);
                battleConsole.pushText("You defeated enemy!");

                return;
            }

            EnemyTurn(player, enemy, battleConsole);
            UpdateCharacterTxt(/*might be a problem*/ enemyName, enemySprites, enemyHpTxt, enemyAttackTxt, enemyDeffenseTxt, currentEnemyIndex, enemies, battleScene);
            UpdateHealthTxt(playerHealthTxt, player, "Your");

            //check if player dies
            if (player.getHP() != 0) return;
            
            sceneHandler.changeToScene(youLostScene);

        });

    healBtn.setButtonAction([&]() {
        Character& enemy = *enemies[currentEnemyIndex];
        Character& player = *characters[currentCharacterIndex];

        int prevHp = player.getHP();
        player.HealSelf();
        int heal = player.getHP() - prevHp;
        battleConsole.pushText("You healed with " + std::to_string(heal) + " points!");

        EnemyTurn(player, enemy, battleConsole);
        UpdateCharacterTxt(/*might be a problem*/ enemyName, enemySprites, enemyHpTxt, enemyAttackTxt, enemyDeffenseTxt, currentEnemyIndex, enemies, battleScene);
        UpdateHealthTxt(playerHealthTxt, player, "Your");

        //check if player dies
        if (player.getHP() != 0) return;

        sceneHandler.changeToScene(youLostScene);
        });


    quitFightBtn.setButtonAction([&enemySprites,&enemies, &characters, &currentCharacterIndex, &characterSprites,&window]()
        {
            //save progress
            SaveProgress(enemySprites, enemies, *characters[currentCharacterIndex], *characterSprites[currentCharacterIndex]);
            window.close();
        });

#pragma endregion

#pragma region YouWinScreen
    youWinScene.onEnter([&characters,&currentCharacterIndex, &youWonText]() {
            Character& player = *characters[currentCharacterIndex];
            
            player.setExp(player.getExp() + 10);
            SaveHighScore(player.getExp());
            DrawHighScore(youWonText);
        
        });
    youWonQuitBtn.setButtonAction([&characters, &currentCharacterIndex, &window]() 
        {
            //save high score
            window.close();
        });

#pragma endregion

#pragma region YouLostScreen

    youLostQuitBtn.setButtonAction([&window]()
        {
            window.close();
        });

#pragma endregion

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                sceneHandler.handleEvent(event, window);
            }
        }

        window.clear(sf::Color::White);
        sceneHandler.update();
        sceneHandler.render(window);
        window.display();
    }
    
    for (auto sprite : enemySprites)
        delete sprite;
    enemySprites.clear();
    for (auto sprite : enemies)
        delete sprite;
    enemies.clear();

    for (auto sprite : characterSprites)
        delete sprite;
    characterSprites.clear();
    for (auto sprite : characters)
        delete sprite;
    characters.clear();
    return 0;
}





