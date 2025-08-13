#pragma once
#include <vector>
#include "character.hpp"
class BattleState
{
public:
	int difficulty;
	int currentIteration;
	int currentEnemyIndex;

	std::shared_ptr<Character> loadedPlayer;
	std::vector<std::shared_ptr<Character>> enemies;
	std::string battleData;
};

