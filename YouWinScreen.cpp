#include "YouWinScreen.h"
#include "sceneHandler.hpp"
#include <fstream>
#include <sstream>
#include "SceneAssets.h"
#include "UIHelper.h"

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
    SceneAssets* assets = SceneAssets::GetInstance();
    const sf::Vector2f btnSize(200.5f, 50.0f);

    gameOverText = UIHelper::CreateText("GameOverTxt", assets->mainFont, "Game Over!", offset, assets->h1FontSize, assets->black);

    offset.y += 55;
    highscoreText = UIHelper::CreateText("highScoreText", assets->mainFont, "LeaderBoard", offset, assets->h2FontSize, assets->black);

    offset.y += 200;
    yourScoreText = UIHelper::CreateText("YourScoreText", assets->mainFont, "Your Score:", offset, assets->h2FontSize, assets->black);

    offset.y = 500;
    quitBtn = UIHelper::CreateButton("QuitBtn", assets->mainFont, "Quit", btnSize, assets->darkColor, offset);

    offset.y += 55;
    backBtn = UIHelper::CreateButton("BackBtn", assets->mainFont, "Back", btnSize, assets->darkColor, offset);

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
        std::string dataPath = SceneAssets::GetInstance()->GetPath("data.cmgt");
		std::ifstream battleSaveFileRead(dataPath);
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
    std::string highScorePath = SceneAssets::GetInstance()->GetPath("HighScores.txt");

    std::ifstream myfileRead(highScorePath);
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
    std::string highScorePath = SceneAssets::GetInstance()->GetPath("HighScores.txt");

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
    
    std::ofstream myfileWrite(highScorePath, std::ios::trunc);

    for (size_t i = 0; i < scoreAmount; i++)
    {
        myfileWrite << std::to_string( highScores[i].score) << ',' << highScores[i].name  << std::endl;
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
    std::ostringstream oss;

    for (size_t i = 0; i < highScores.size(); i++)
    {
        oss << (i + 1) << '.' << highScores[i].name << ": " << highScores[i].score << '\n';
    }

    textMesh.setText(oss.str());
}