#include "YouWinScreen.h"
#include "sceneHandler.hpp"
#include <fstream>

extern std::string folderPrefix;

YouWinScreen::YouWinScreen(std::string identifier, SceneHandler& manager) : Scene(identifier, manager, sf::Color(7, 79, 87)), lastScore(0,"")
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
    sf::Vector2f offset((float)windowSize.x / 2 - 100, 100);

	gameOverText = std::make_shared<TextObject>("GameOverTxt", manager->mainFont, "Game Over!");
	gameOverText->setPosition(offset);
	gameOverText->setCharacterSize(42);
	gameOverText->setFillColor(manager->black);

    offset.y += 55;
    highscoreText = std::make_shared<TextObject>("highScoreText", manager->mainFont, "LeaderBoard");
    highscoreText->setPosition(offset);
    highscoreText->setCharacterSize(26);
    highscoreText->setFillColor(manager->black);

    offset.y += 200;
    yourScoreText = std::make_shared<TextObject>("YourScoreText", manager->mainFont, "Your Socre:");
    yourScoreText->setPosition(offset);
    yourScoreText->setCharacterSize(26);
    yourScoreText->setFillColor(manager->black);

    offset.y = 500;
	quitBtn = std::make_shared<Button>("QuitBtn", manager->mainFont, "Quit",
		sf::Vector2f(200.5f, 50.0f), manager->darkColor);
	quitBtn->setPosition(offset);

    offset.y += 55;
	backBtn = std::make_shared<Button>("BackBtn", manager->mainFont, "Back",
		sf::Vector2f(200.5f, 50.0f), manager->darkColor);
	backBtn->setPosition(offset);

	addGameObject(gameOverText);
	addGameObject(yourScoreText);
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
        
        lastScore = ScoreData(player.getExp(), player.getName());
        
        yourScoreText->setText("Your Score: " + lastScore.name + ". " + std::to_string(lastScore.score));

        std::vector<ScoreData> highScores = LoadHighscore();
	    SaveHighScore(lastScore, highScores);
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