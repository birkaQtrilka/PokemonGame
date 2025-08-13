#pragma once
#include "EnemyRandomValues.h"

constexpr auto DIFFICULTIES = 5;

class DifficultyManager
{
public:
	static EnemyRandomValues GetRandomValues(int difficulty);
	//const int DIFFICULTIES;

private:
	static EnemyRandomValues difficultyValues[DIFFICULTIES];

};

