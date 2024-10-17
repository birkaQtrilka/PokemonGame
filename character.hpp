#pragma once

#include <string>

class Character {
    private:
        std::string name;
        std::string spriteFile;
        int hp;
        int attack;
        int defense;

    public:
        Character(std::string name, std::string spriteFile, 
                    int hp, int attack, int defense);
        Character();
        ~Character();

        int attackCharacter(const Character& character) const;
        bool takeDamage(int damage);

        int getAttack() const;
        void setAttack(int attack);

        int getDefense() const;
        void setDefense(int defense);

        int getHP() const;
        void setHP(int hp);

        std::string getName() const;
        void setName(std::string name);

        std::string getSpriteFile() const;
        void setSpriteFile(std::string name);

        void HealSelf();
};


