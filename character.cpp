#pragma once

#include <sstream>
#include "character.hpp"
#include "SceneAssets.h"

Character::Character(std::string data) :
    GameObject("character")
    //TODO: position, scale, 
{ 
    Deserialize(data);
}


Character::~Character() {
    delete sprite;
}

void Character::Serialize(std::ostringstream& stream) const
{
    const char delim = ',';
    stream << getName() << delim
        << getSpriteFile() << delim
        << getHP() << delim
        << getAttack() << delim
        << getDefense() << delim
        << getExp() << '\n';
}

void Character::Deserialize(const std::string& line)
{
    std::vector<std::string> strings = split(line, ',');
    name = strings[0];
    std::size_t found = strings[1].find_last_of("/\\");
    std::string filePath = SceneAssets::GetInstance()->GetPath( strings[1].substr(found + 1));
    sprite = new SpriteObject(filePath, filePath);

    setHP(std::stoi(strings[2]));
    setAttack(std::stoi(strings[3]));
    setDefense(std::stoi(strings[4]));
    setExp(std::stoi(strings[5]));
}

void Character::render(sf::RenderWindow& window)
{
    sprite->render(window);
}

void Character::update()
{
    sprite->update();
}

void Character::setPosition(sf::Vector2f position)
{
    sprite->setPosition(position);
}

void Character::setScale(sf::Vector2f scale)
{
    sprite->setScale(scale);
}

int Character::attackCharacter(const Character& character) const {
    //Just a random function
    
    //attack + 0~3 
    int attackPower = attack + (rand() % 4); 
    // defense + -1~1
    int defensePower = character.getDefense() + (-1 + rand() % 3); 

    return std::max(attackPower - defensePower, 0);
}

int Character::HealSelf()
{
    int heal = 1 + rand() & this->defense + 1;
    this->hp += heal;
    return heal;
}

void Character::UpdateText(std::shared_ptr<TextObject> nameTxt, std::shared_ptr<TextObject> hpTxt, std::shared_ptr<TextObject> attackTxt, std::shared_ptr<TextObject> deffenseTxt) const
{
    nameTxt->setText(getName());
    attackTxt->setText("Attack: " + std::to_string(getAttack()));
    hpTxt->setText("Hp: " + std::to_string(getHP()));
    deffenseTxt->setText("Defense: " + std::to_string(getDefense()));

}

std::string Character::getSpriteFile() const
{
    return sprite->getSpriteFile();
}

bool Character::takeDamage(int damage) {
    hp = std::max(hp - damage, 0);
    return hp == 0;
}

int Character::getAttack() const {
    return this->attack;
}

void Character::setAttack(int attack) {
    this->attack = attack;
}

int Character::getDefense() const {
    return this->defense;
}

int Character::getHP() const {
    return this->hp;
}

int Character::getExp() const {
    return this->exp;
}

std::string Character::getName() const {
    return this->name;
}

void Character::setDefense(int defense) {
    this->defense = defense;
}

void Character::setHP(int hp) {
    this->hp = (hp > 0) ? hp : 0;
}

void Character::setExp(int exp) {
    this->exp = (exp > 0) ? exp : 0;
}

void Character::setName(std::string name) {
    this->name = name;
}



