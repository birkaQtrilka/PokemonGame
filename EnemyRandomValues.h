#pragma once
#include <random>
struct EnemyRandomValues
{
public:
	EnemyRandomValues(int minHp,int maxHp, int minAtck, int maxAtck, int minDfns, int maxDfns, int minExp, int maxExp);
	~EnemyRandomValues();

	std::uniform_int_distribution<> health;
	std::uniform_int_distribution<> attack;
	std::uniform_int_distribution<> defense;
	std::uniform_int_distribution<> exp;

};

