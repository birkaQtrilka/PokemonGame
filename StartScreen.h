#pragma once

#include "Scene.hpp"
#include "textObject.hpp"
#include "button.hpp"

class StartScreen :
    public Scene
{
private:
    std::shared_ptr<TextObject> gameTitle;
    std::shared_ptr<Button> startNewBtn;
    std::shared_ptr<Button> continueBtn;
    std::shared_ptr<Button> quitBtn;

public:
    StartScreen(std::string identifier, SceneHandler& manager);
    ~StartScreen();
private:
    void SetUpInterface();
    void SetUpBehavior();

};

