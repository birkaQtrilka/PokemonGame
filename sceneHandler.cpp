#pragma once

#include "sceneHandler.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

extern std::string folderPrefix;

SceneHandler::SceneHandler(sf::RenderWindow& window) : 
    enemyImages({ folderPrefix +"plant.png", folderPrefix + "mushroom.png",
        folderPrefix + "amoeba.png", folderPrefix + "virus.png" }), 
    scene(nullptr)
{
    this->window = &window;
    mainFont.loadFromFile(folderPrefix + "Lato-Regular.ttf");
;}

SceneHandler::~SceneHandler() 
{ }

void SceneHandler::render(sf::RenderWindow& window) const {
    this->scene->render(window);
}

void SceneHandler::update() {
    this->scene->update();
}

void SceneHandler::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    this->scene->handleEvent(event, window);
}

void SceneHandler::changeToScene(const std::string& sceneID) 
{
    Scene* newScene = scenes[sceneID];
    if (this->scene != nullptr && this->scene->exitAction)
        this->scene->exitAction();
    
    this->scene = newScene;

    if(newScene->enterAction)
        scene->enterAction();
}

void SceneHandler::addScene(Scene& scene)
{
    this->scenes.emplace(scene.getIdentifier(), &scene);
}

void SceneHandler::GenerateBattleFile(const Character& player) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> r_hp(10, 17);
    std::uniform_int_distribution<> r_attack(1, 5);
    std::uniform_int_distribution<> r_defense(1, 3);
    std::uniform_int_distribution<> r_enemySprite(0, 1);
    std::uniform_int_distribution<> r_enemyCount(1, 4);
    std::uniform_int_distribution<> r_enemyExp(10, 20);

    std::ofstream myfileWrite(folderPrefix + "data.cmgt", std::ios::trunc);
    int enemyCount = r_enemyCount(gen);

    myfileWrite << player.getName() + ',' +
        player.getSpriteFile() + ',' +
        std::to_string(player.getHP()) + ',' +
        std::to_string(player.getAttack()) + ',' +
        std::to_string(player.getDefense()) + ',' +
        std::to_string(player.getExp())
        << std::endl;

    myfileWrite << enemyCount << std::endl;

    for (size_t i = 0; i < enemyCount; i++)
    {
        myfileWrite << "Enemy" + std::to_string(enemyCount - i) + ',' +
            enemyImages[r_enemySprite(gen)] + ',' +
            std::to_string(r_hp(gen)) + ',' +
            std::to_string(r_attack(gen)) + ',' +
            std::to_string(r_defense(gen)) + ',' +
            std::to_string(r_enemyExp(gen))
            << std::endl;
    }
    myfileWrite.close();
}



