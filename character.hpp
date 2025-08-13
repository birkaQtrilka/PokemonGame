#pragma once

#include <string>
#include "spriteObject.hpp"
#include "textObject.hpp"
#include <sstream>

std::vector<std::string> split(const std::string& s, char delim);

class Character : public GameObject{
    private:
        SpriteObject* sprite;
        std::string name;
        int hp;
        int attack;
        int defense;
        int exp;

    public:
        Character(std::string data);

        ~Character();

        void Serialize(std::ostringstream& stream) const;
        void Deserialize(const std::string& line);

        void render(sf::RenderWindow& window) override;
        void update() override;
        void setPosition(sf::Vector2f position);
        void setScale(sf::Vector2f scale);

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

        int HealSelf();

        void UpdateText(std::shared_ptr<TextObject> nameTxt, std::shared_ptr<TextObject> hpTxt, std::shared_ptr<TextObject> attackTxt, std::shared_ptr<TextObject> deffenseTxt) const;
        std::string getSpriteFile() const;

};


