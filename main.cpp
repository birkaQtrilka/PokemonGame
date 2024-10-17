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

void characterAttackItself() {
    // GameObject& character = scene.getGameObjectByName("character");
    //Make the character attack it self
    printf("Hello world!\n");
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void GenerateRandomEnemies(std::mt19937& gen, std::vector<SpriteObject*>& enemySprites)
{
    std::uniform_int_distribution<> hpRandom(10, 17);
    std::uniform_int_distribution<> attackRandom(1, 5);
    std::uniform_int_distribution<> defenseRandom(1, 3);
    std::uniform_int_distribution<> radnomEnemySprite(0, enemySprites.size()-1);
    std::uniform_int_distribution<> r_enemyCount(1, 4);
    
    std::ofstream myfileWrite("BattleSave.txt", std::ios::trunc);
    int enemyCount = r_enemyCount(gen);
    
    myfileWrite << enemyCount << std::endl;

    for (size_t i = 0; i < enemyCount; i++)
    {
        myfileWrite << "Enemy" + std::to_string(i) + ',' +
            enemySprites[radnomEnemySprite(gen)]->getSpriteFile() + ',' +
            std::to_string(hpRandom(gen)) + ',' + 
            std::to_string(attackRandom(gen)) + ',' + 
            std::to_string(defenseRandom(gen)) 
            << std::endl;
    }
    
}

void SetCharacter(Character& character, std::string& line, std::vector<SpriteObject*>& sprites)
{
    std::vector<std::string> strings = split(line, ',');

    character.setName(strings[0]);

    character.setSpriteFile(strings[1]);
    SpriteObject* sprite = new SpriteObject(strings[1], strings[1]);
    sprite->SetActive(false);
    sprites.push_back(sprite);

    character.setHP(std::stoi(strings[2]));
    character.setAttack(std::stoi(strings[3]));
    character.setDefense(std::stoi(strings[4]));

}

void UpdateCharacterTxt(int previousIndex,
    TextObject& nameTxt, std::vector<SpriteObject*>& sprites, TextObject& hpTxt,
    TextObject& attackTxt, TextObject& deffenseTxt, int& currentIndex,
    std::vector<Character*>& characters, Scene& scene)
{
    Character& currentCharacter = *characters[currentIndex];
    nameTxt.setText(currentCharacter.getName());

    sprites[previousIndex]->SetActive(false);
    sprites[currentIndex]->SetActive(true);

    attackTxt.setText(std::to_string(currentCharacter.getAttack()));
    hpTxt.setText(std::to_string(currentCharacter.getHP()));
    deffenseTxt.setText(std::to_string(currentCharacter.getDefense()));
};

void EnemyTurn(Character& player, Character& enemy)
{
    printf("Enemy turn!\n");
    printf("Enemy ");
    switch ((rand() % 3))
    {
    case 0:
    {
        int dmg = enemy.attackCharacter(player);
        player.setHP(player.getHP() - dmg );
        printf("attacked with %d damage!\n", dmg);
    }
        break;
    case 1:
        enemy.HealSelf();
        break;
    case 2:
        printf("did nothing!\n");
        break;
    }
    printf("Enemy Health: %d\n", enemy.getHP());
    printf("Your Health: %d\n", player.getHP());
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML with Scenes!");
    std::random_device rd;
    std::mt19937 gen(rd());

    Scene startScreen("StartScreen");
    Scene characterScreen("CharacterSelectionScene");
    Scene battleScene("BattleScene");
    
    std::vector<SpriteObject*> enemySprites;
    enemySprites.push_back(new SpriteObject("enemy1","enemy1.png"));
    enemySprites.push_back(new SpriteObject("enemy2","enemy2.png"));
    std::uniform_int_distribution<> r_enemyAction(0,2);

    

    std::vector<Character*> characters;
    std::vector<SpriteObject*> characterSprites;
    std::vector<Character*> enemies;
    int currentCharacterIndex = 0;

    Scene* currentScene = &startScreen;

    sf::Font font;
    font.loadFromFile("Lato-Regular.ttf");
    sf::Color darkColor = sf::Color(71, 82, 94, 255);
    sf::Color darkGreyColor = sf::Color(132, 146, 166, 255);
    sf::Color lightGreyColor = sf::Color(129, 144, 165, 255);
    sf::Color programmerColor = sf::Color(200, 0, 0, 255);

    auto loadEnemies = [&enemies, &enemySprites]()
        {
            std::ifstream battleSaveFileRead("BattleSave.txt");
            std::string line;
            std::getline(battleSaveFileRead, line);
            int enemyCount = std::stoi(line);
            for (size_t i = 0; i < enemyCount; i++)
            {
                if (battleSaveFileRead.fail()) return 0;

                std::getline(battleSaveFileRead, line);
                Character* currentenemy = new Character();
                enemies.push_back(currentenemy);
                SetCharacter(*currentenemy, line, enemySprites);
            }
            battleSaveFileRead.close();
        };

#pragma region StartScreenSetup
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

    Button attackBtn("attackItselfButton", font, "Attack",
        sf::Vector2f(192.5f, 50.0f), darkColor);
    attackBtn.setPosition(sf::Vector2f(508.0f, 500.0f));
#pragma endregion


#pragma region BattleScene
    //at window close, save enemies state 
    //show first enemy 
    int currentEnemyIndex = 0;

    for (SpriteObject* enemySprite : enemySprites)
    {
        enemySprite->setPosition(sf::Vector2f(135.0f, 90.0f));
        enemySprite->setScale(sf::Vector2f(2.5f, 2.5f));
        battleScene.addGameObject(*enemySprite);
    }

    //turn based loop-----------------------------------------
    

    attackBtn.setButtonAction([&]()
        {
            currentScene = &battleScene;
            //go in file and change hp
            Character& enemy = *enemies[currentEnemyIndex];
            Character& player = *characters[currentCharacterIndex];
            enemy.setHP(enemy.getHP() - player.attackCharacter(enemy));
            UpdateCharacterTxt(0/*might be a problem*/, enemyName, enemySprites, enemyHpTxt, enemyAttackTxt, enemyDeffenseTxt, currentEnemyIndex, enemies, battleScene);
            EnemyTurn(player, enemy);

        });

    battleScene.addGameObject(enemyName);
    battleScene.addGameObject(enemyHpTxt);
    battleScene.addGameObject(enemyAttackTxt);
    battleScene.addGameObject(enemyDeffenseTxt);
    battleScene.addGameObject(attackBtn);

#pragma endregion

#pragma region StartScreen

    startNewBtn.setButtonAction([&]()
        {
            currentScene = &characterScreen;
            UpdateCharacterTxt(currentCharacterIndex, characterName,
                characterSprites, hpText, attackText, defenseText
                , currentCharacterIndex, characters, characterScreen);
        });

    

    continueBtn.setButtonAction([&]()
        {
            currentScene = &battleScene;

            loadEnemies();//could become load save
            UpdateCharacterTxt(0, enemyName, enemySprites, enemyHpTxt, enemyAttackTxt, enemyDeffenseTxt, currentEnemyIndex, enemies, battleScene);

        });

    

    quitBtn.setButtonAction([&currentScene, &window]()
        {
            window.close();
        });

    startScreen.addGameObject(gameTitle);
    startScreen.addGameObject(continueBtn);
    startScreen.addGameObject(startNewBtn);
    startScreen.addGameObject(quitBtn);


#pragma endregion

#pragma region Character Screen
    std::ifstream characterSelectionFileRead("CharacterSelection.txt");
    std::string line;
    std::getline(characterSelectionFileRead, line);
    int characterCount = std::stoi(line);

    //Character character("Dude", "head.png", 10, 2, 2);
    for (size_t i = 0; i < characterCount; i++)
    {
        if (characterSelectionFileRead.fail()) return 0;
        
        std::getline(characterSelectionFileRead, line);
        Character* currentCharacter = new Character();
        characters.push_back(currentCharacter);
        SetCharacter(*currentCharacter, line, characterSprites);
    }
    characterSelectionFileRead.close();
    //create n sprites, just add and remove them during update functions
    
    for (SpriteObject* characterAvatar : characterSprites)
    {
        characterAvatar->setPosition(sf::Vector2f(135.0f, 90.0f));
        characterAvatar->setScale(sf::Vector2f(2.5f, 2.5f));
    }

    

    
    Button charLeftBtn("CharLeftBtn", font, "<", sf::Vector2f(192.5f, 50.0f), darkColor);
    charLeftBtn.setPosition(sf::Vector2f(50.0f, 440.0f));

    charLeftBtn.setButtonAction([&]() {

        int previousIndex = currentCharacterIndex;
        if (currentCharacterIndex != 0)
            currentCharacterIndex--;
        UpdateCharacterTxt(previousIndex,characterName,
            characterSprites,hpText,attackText,defenseText
            ,currentCharacterIndex,characters,characterScreen);

    });

    Button charRightBtn("CharLeftBtn", font, ">", sf::Vector2f(192.5f, 50.0f), darkColor);
    charRightBtn.setPosition(sf::Vector2f(250.0f, 440.0f));

    charRightBtn.setButtonAction([&]() {
        int previousIndex = currentCharacterIndex;
        if (currentCharacterIndex != characterCount-1)
            currentCharacterIndex++;
        UpdateCharacterTxt(previousIndex, characterName,
            characterSprites, hpText, attackText, defenseText
            , currentCharacterIndex, characters, characterScreen);
    });

    Button quitButton("quitButton", font, "QUIT", sf::Vector2f(192.5f, 50.0f), darkColor);
    quitButton.setPosition(sf::Vector2f(108.0f, 540.0f));

    quitButton.setButtonAction([&window] () {
        window.close();
    });
    
    Button playBtn("quitButton", font, "Play", sf::Vector2f(192.5f, 50.0f), darkColor);
    playBtn.setPosition(sf::Vector2f(108.0f, 640.0f));

    

    playBtn.setButtonAction([&]()
    {
            currentScene = &battleScene;
            GenerateRandomEnemies(gen, enemySprites);
            loadEnemies();
            UpdateCharacterTxt(0, enemyName, enemySprites, enemyHpTxt, enemyAttackTxt, enemyDeffenseTxt, currentEnemyIndex, enemies, battleScene);

    });

    characterScreen.addGameObject(characterName);
    for (auto characterAvatar : characterSprites)
    {
        characterScreen.addGameObject(*characterAvatar);
    }
    characterScreen.addGameObject(hpText);
    characterScreen.addGameObject(attackText);
    characterScreen.addGameObject(defenseText);
    characterScreen.addGameObject(quitButton);
    characterScreen.addGameObject(charLeftBtn);
    characterScreen.addGameObject(charRightBtn);
    characterScreen.addGameObject(playBtn);


#pragma endregion



    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                currentScene->handleEvent(event, window);
            }
        }

        window.clear(sf::Color::White);
        currentScene->update();
        currentScene->render(window);
        window.display();
    }
    for (auto sprite : characterSprites)
        delete sprite;
    characterSprites.clear();
    for (auto sprite : enemySprites)
        delete sprite;
    enemySprites.clear();

    for (auto sprite : enemies)
        delete sprite;
    enemies.clear();
    for (auto sprite : characters)
        delete sprite;
    characters.clear();
    return 0;
}





