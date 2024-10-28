#pragma once

#include "scene.hpp"
#include "textObject.hpp"
#include "button.hpp"
#include "character.hpp"

class CharacterSelectScreen :
    public Scene
{
private:
    std::shared_ptr<TextObject> characterName;
    std::shared_ptr<TextObject> hpText;
    std::shared_ptr<TextObject> attackText;
    std::shared_ptr<TextObject> defenseText;
    std::shared_ptr<Button> charLeftBtn;
    std::shared_ptr<Button> charRightBtn;
    std::shared_ptr<Button> quitButton;
    std::shared_ptr<Button> backButton;
    std::shared_ptr<Button> playBtn;
    std::vector<std::shared_ptr<Character>> characters;
    int currentCharacterIndex;

public:
    CharacterSelectScreen(std::string identifier, SceneHandler& manager);
    ~CharacterSelectScreen();
private:
    void SetUpInterface();
    void SetUpBehavior();

};

