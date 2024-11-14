#pragma once
#include "sceneHandler.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "DifficultyManager.h"

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

void SceneHandler::GenerateBattleFile(const Character& player, const int difficulty) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    EnemyRandomValues r = DifficultyManager::GetRandomValues(difficulty);
    std::uniform_int_distribution<> randomSprite(0, enemyImages.size() - 1);

    int enemyCount = 5;

    std::ofstream myfileWrite(folderPrefix + "data.cmgt", std::ios::trunc);

    myfileWrite << player.getName() + ',' +
        player.getSpriteFile() + ',' +
        std::to_string(player.getHP()) + ',' +
        std::to_string(player.getAttack()) + ',' +
        std::to_string(player.getDefense()) + ',' +
        std::to_string(player.getExp())
        << std::endl;
    myfileWrite << enemyCount << std::endl;
    myfileWrite << 0 << std::endl;
    myfileWrite << difficulty << std::endl;
    myfileWrite << 0 << std::endl;

    for (size_t i = 0; i < enemyCount; i++)
    {
        myfileWrite << "Enemy" + std::to_string(i) + ',' +
            enemyImages[randomSprite(gen)] + ',' +
            std::to_string(r.health(gen)) + ',' +
            std::to_string(r.attack(gen)) + ',' +
            std::to_string(r.defense(gen)) + ',' +
            std::to_string(r.exp(gen))
            << std::endl;
    }
    myfileWrite.close();
}



