#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

#include "scene.hpp"
#include "sceneHandler.hpp"
#include "Console.h"
#include "StartScreen.h"
#include "CharacterSelectScreen.h"
#include "BattleScreen.h"
#include "YouWinScreen.h"
#include "Utils.h"
#include "SceneAssets.h"

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML with Scenes!");

    std::string musicPath = SceneAssets::GetInstance()->GetPath("8BitExplosion.mp3");
    sf::SoundBuffer mainMusicBuffer;
    mainMusicBuffer.loadFromFile(musicPath);

    sf::Sound mainMusic;
    mainMusic.setBuffer(mainMusicBuffer);
    mainMusic.setLoop(true);
    mainMusic.setVolume(30);
    mainMusic.play();

    SceneHandler sceneHandler(window);

    StartScreen startScreen("StartScene", sceneHandler);
    CharacterSelectScreen characterScreen("CharacterScene", sceneHandler);
    BattleScreen battleScene("BattleScene", sceneHandler);
    YouWinScreen youWinScene("YouWinScene", sceneHandler);
    
    sceneHandler.addScene(startScreen);
    sceneHandler.addScene(characterScreen);
    sceneHandler.addScene(battleScene);
    sceneHandler.addScene(youWinScene);

    sceneHandler.changeToScene("StartScene");

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
    
    return 0;
}





