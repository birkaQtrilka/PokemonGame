#include "character.hpp"
#include <stdlib.h>  

#include <algorithm>

Character::Character(std::string name, int spriteId, int hp, int attack, int defense) :
    name(name), spriteID(spriteId), hp(hp), attack(attack), defense(defense), exp(0)
{ }
Character::Character() : name("name"), spriteID(0), hp(10), attack(1), defense(1), exp(0)
{ }
Character::~Character() { }

int Character::attackCharacter(const Character& character) const {
    //Just a random function
    
    //attack + 0~3 
    int attackPower = attack + rand() % 4; 
    // defense + -1~1
    int defensePower = character.getDefense() + (-1 + rand() % 3); 

    return std::max(attackPower - defensePower, 0);
}

void Character::HealSelf()
{
    int heal = 1 + rand() & this->defense;
    this->hp += heal;
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

int Character::getSpriteID() const {
    return this->spriteID;
}

void Character::setSpriteID(int ID) {
    this->spriteID = ID;
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

