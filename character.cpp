#pragma once

//#include <fstream>
#include <sstream>
#include "character.hpp"
//#include <algorithm>
//#include <stdlib.h>  


Character::Character(std::string data) :
    GameObject("character")
    //TODO: position, scale
{ 
    std::vector<std::string> strings = split(data, ',');//do I need to call strings.clear() at the end?
    name = strings[0];
    sprite = new SpriteObject(strings[1], strings[1]);

    setHP(std::stoi(strings[2]));
    setAttack(std::stoi(strings[3]));
    setDefense(std::stoi(strings[4]));
    setExp(std::stoi(strings[5]));

    strings.clear();
}

Character::~Character() {
    delete sprite;
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



