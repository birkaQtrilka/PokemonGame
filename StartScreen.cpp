#include "StartScreen.h"
#include "sceneHandler.hpp"
#include "SceneAssets.h"
#include "UIHelper.h"

StartScreen::StartScreen(std::string identifier, SceneHandler& manager) : Scene(identifier, manager, sf::Color(7, 79, 87))
{
    SetUpInterface();
    SetUpBehavior();
}

StartScreen::~StartScreen()
{

}

void StartScreen::SetUpInterface()
{
    SceneAssets* assets = SceneAssets::GetInstance();
    const sf::Vector2f btnSize(192.5f, 50.0f);

    gameTitle = UIHelper::CreateText("GameNameText", assets->mainFont, "PogaemonCo", sf::Vector2f(109.0f, 34.0f), assets->h2FontSize, assets->programmerColor);

    startNewBtn = UIHelper::CreateButton("attackItselfButton", assets->mainFont, "Start new", btnSize, assets->darkColor, sf::Vector2f(108.0f, 400.0f));
    continueBtn = UIHelper::CreateButton("attackItselfButton", assets->mainFont, "Continue", btnSize, assets->darkColor, sf::Vector2f(108.0f, 500.0f));
    quitBtn = UIHelper::CreateButton("QuitBtn", assets->mainFont, "Quit", btnSize, assets->darkColor, sf::Vector2f(108.0f, 600.0f));

    addGameObject(gameTitle);
    addGameObject(continueBtn);
    addGameObject(startNewBtn);
    addGameObject(quitBtn);
}

void StartScreen::SetUpBehavior()
{
    startNewBtn->setButtonAction([&]()
    {
        manager->changeToScene("CharacterScene");

    });

    continueBtn->setButtonAction([&]()
    {
        manager->changeToScene("BattleScene");
    });
    
    quitBtn->setButtonAction([&]()
    {
        manager->window->close();
    });
}
