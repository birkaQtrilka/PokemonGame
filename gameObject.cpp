#include "gameObject.hpp"
#include "scene.hpp"

unsigned int GameObject::_id = 0;

GameObject::GameObject(std::string identifier) : identifier(identifier + std::to_string(_id++)), _enabled(true){ }
 
GameObject::GameObject(const GameObject& other) : identifier(other.getIdentifier()) { }

GameObject::~GameObject() { }

std::string GameObject::getIdentifier() const {
    return this->identifier;
}

void GameObject::SetActive(bool val)
{
    _enabled = val;
}

bool GameObject::GetActive() const
{
    return _enabled;
}