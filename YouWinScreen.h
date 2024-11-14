#pragma once

#include "scene.hpp"
#include "button.hpp"
#include "textObject.hpp"

struct ScoreData
{
public:
    int score;
    std::string name;
    ScoreData(int s, std::string n) : score(s), name(n)
    {
    }
};

std::vector<std::string> split(const std::string& s, char delim);

class YouWinScreen :
    public Scene
{
private:
    std::shared_ptr<TextObject> gameOverText;
    std::shared_ptr<TextObject> highscoreText;
    std::shared_ptr<TextObject> yourScoreText;
    std::shared_ptr<Button> quitBtn;
    std::shared_ptr<Button> backBtn;

    ScoreData lastScore;

    const int HighScoreListSize = 5;
public:
    YouWinScreen(std::string identifier, SceneHandler& manager);
    ~YouWinScreen();
private:
    void SetUpInterface();
    void SetUpBehavior();
    std::vector<ScoreData> LoadHighscore() const;
    void DrawHighScore(TextObject& textMesh, const std::vector<ScoreData>& highScores);
    void SaveHighScore(const ScoreData data, std::vector<ScoreData>& highScores);
    void BubbleSort(std::vector<ScoreData>& arr);
};

