#pragma once

#include <string>

class Character {
    private:
        std::string name;
        int spriteID;
        int hp;
        int attack;
        int defense;
        int exp;

    public:
        Character(std::string name, int spriteId, 
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

        int getExp() const;
        void setExp(int hp);

        std::string getName() const;
        void setName(std::string name);

        int getSpriteID() const;
        void setSpriteID(int ID);

        void HealSelf();
};


