#include "sceneHandler.hpp"
#include "YouLostScreen.h"

YouLostScreen::YouLostScreen(std::string identifier, SceneHandler& manager) : Scene(identifier, manager, sf::Color(7, 79, 87))
{
	SetUpInterface();
	SetUpBehavior();
}

YouLostScreen::~YouLostScreen()
{

}

void YouLostScreen::SetUpInterface()
{
	youLostText = std::make_shared<TextObject>("YouWonText", manager->mainFont, "You Lost!");
	youLostText->setPosition(sf::Vector2f(509.0f, 34.0f));
	youLostText->setCharacterSize(26);
	youLostText->setFillColor(manager->programmerColor);
	
	quitBtn = std::make_shared<Button>("QuitBtn", manager->mainFont, "Quit",
	    sf::Vector2f(509.5f, 50.0f), manager->darkColor);
	quitBtn->setPosition(sf::Vector2f(108.0f, 600.0f));
	
	backBtn = std::make_shared<Button>("BackBtn", manager->mainFont, "Back",
		sf::Vector2f(509.5f, 50.0f), manager->darkColor);
	backBtn->setPosition(sf::Vector2f(108.0f, 700.0f));

	addGameObject(youLostText);
	addGameObject(quitBtn);
	addGameObject(backBtn);
}

void YouLostScreen::SetUpBehavior()
{
	quitBtn->setButtonAction([&]()
	{
	    manager->window->close();
	});

	backBtn->setButtonAction([&]()
		{
			manager->changeToScene("BattleScene");
		});
}



