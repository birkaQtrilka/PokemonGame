#include "character.hpp"
#include <stdlib.h>  

#include <algorithm>

Character::Character(std::string name, std::string spriteFile, int hp, int attack, int defense) :
    name(name), spriteFile(spriteFile), hp(hp), attack(attack), defense(defense)
{ }
Character::Character() : name("name"), spriteFile("head.png"), hp(10), attack(1), defense(1)
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
    printf("healed with %d hp!\n", heal);
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

std::string Character::getName() const {
    return this->name;
}

std::string Character::getSpriteFile() const {
    return this->spriteFile;
}
void Character::setSpriteFile(std::string spriteFile) {
    this->spriteFile = spriteFile;
}
void Character::setDefense(int defense) {
    this->defense = defense;
}

void Character::setHP(int hp) {
    this->hp = (hp > 0) ? hp : 0;
}

void Character::setName(std::string name) {
    this->name = name;
}

