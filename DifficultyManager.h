#pragma once
#include "EnemyRandomValues.h"

#define DIFFICULTIES 5

static class DifficultyManager
{
public:
	static EnemyRandomValues GetRandomValues(int difficulty);
	//const int DIFFICULTIES;

private:
	static EnemyRandomValues difficultyValues[DIFFICULTIES];

};

