#pragma once

#include "scene.hpp"
#include "button.hpp"
#include "textObject.hpp"

class YouLostScreen :
    public Scene
{
private:
    std::shared_ptr<TextObject> youLostText;
    std::shared_ptr<Button> quitBtn;
    std::shared_ptr<Button> backBtn;
public:
    YouLostScreen(std::string identifier, SceneHandler& manager);
    ~YouLostScreen();
private:
    void SetUpInterface();
    void SetUpBehavior();
};

