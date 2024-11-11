#include "YouWinScreen.h"
#include "sceneHandler.hpp"
#include <fstream>

extern std::string folderPrefix;

YouWinScreen::YouWinScreen(std::string identifier, SceneHandler& manager) : Scene(identifier, manager, sf::Color(7, 79, 87))
{
    SetUpInterface();
    SetUpBehavior();
}

YouWinScreen::~YouWinScreen()
{

}

void YouWinScreen::SetUpInterface()
{
    auto windowSize = manager->window->getSize();;
    sf::Vector2f offset((float)windowSize.x / 2 - 100, 0);

	youWonText = std::make_shared<TextObject>("YouWonText", manager->mainFont, "You Won!");
	youWonText->setPosition(sf::Vector2f(offset.x, 100));
	youWonText->setCharacterSize(26);
	youWonText->setFillColor(manager->darkGreyColor);

    highscoreText = std::make_shared<TextObject>("highScoreText", manager->mainFont, "LeaderBoard");
    highscoreText->setPosition(sf::Vector2f(offset.x, 140.0f));
    highscoreText->setCharacterSize(26);
    highscoreText->setFillColor(manager->darkGreyColor);

	quitBtn = std::make_shared<Button>("QuitBtn", manager->mainFont, "Quit",
		sf::Vector2f(200.5f, 50.0f), manager->darkColor);
	quitBtn->setPosition(sf::Vector2f(offset.x, 400.0f));

	backBtn = std::make_shared<Button>("BackBtn", manager->mainFont, "Back",
		sf::Vector2f(200.5f, 50.0f), manager->darkColor);
	backBtn->setPosition(sf::Vector2f(offset.x, 455.0f));

	addGameObject(youWonText);
	addGameObject(highscoreText);
	addGameObject(quitBtn);
	addGameObject(backBtn);
}

void YouWinScreen::SetUpBehavior()
{
	onEnter([&]() 
	{
		std::ifstream battleSaveFileRead(folderPrefix + "data.cmgt");
		std::string line;
		std::getline(battleSaveFileRead, line);

		if (battleSaveFileRead.fail()) return;
	    
        Character player(line);
        
        std::vector<ScoreData> highScores = LoadHighscore();
	    SaveHighScore(ScoreData(player.getExp(),player.getName()), highScores);
        DrawHighScore(*highscoreText, highScores);
        highScores.clear();

	});

	quitBtn->setButtonAction([&]()
	{
		manager->window->close();
	});

	backBtn->setButtonAction([&]()
    {
    	manager->changeToScene("BattleScene");
    });

}

std::vector<ScoreData> YouWinScreen::LoadHighscore() const
{
    std::ifstream myfileRead(folderPrefix + "HighScores.txt");
    std::string line;
    std::vector<ScoreData> highScores;

    int scoreAmount = 0;

    while (std::getline(myfileRead, line) && scoreAmount <= HighScoreListSize)
    {
        std::vector<std::string> data = split(line,',');

        highScores.push_back(ScoreData(std::stoi(data[0]), data[1]));
        scoreAmount++;
    }
    myfileRead.close();
    return highScores;
}

void YouWinScreen::SaveHighScore(const ScoreData data, std::vector<ScoreData>& highScores)
{
    int scoreAmount = highScores.size();

    if (scoreAmount < HighScoreListSize)
    {
        highScores.push_back(data);
        scoreAmount++;
    }
    else if (data.score > highScores[HighScoreListSize -1].score)
    {
        highScores[HighScoreListSize - 1] = data;
    }

    BubbleSort(highScores);
    
    std::ofstream myfileWrite(folderPrefix + "HighScores.txt", std::ios::trunc);

    for (size_t i = 0; i < scoreAmount; i++)
    {
        myfileWrite << std::to_string( highScores[i].score) + ',' + highScores[i].name  << std::endl;
    }
    myfileWrite.close();
}

void YouWinScreen::BubbleSort(std::vector<ScoreData>& arr) {
    bool swapped;
    int size = arr.size();

    for (int i = 0; i < size - 1; i++)
    {
        swapped = false;
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j].score < arr[j + 1].score) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) break;
    }
}

void YouWinScreen::DrawHighScore(TextObject& textMesh, const std::vector<ScoreData>& highScores)
{
    std::string text;

    for (size_t i = 0; i < highScores.size(); i++)
    {
        text += std::to_string(i + 1) + '.' + highScores[i].name + ": " +  std::to_string(highScores[i].score) + '\n';
    }

    textMesh.setText(text);
}