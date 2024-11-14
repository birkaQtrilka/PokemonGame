#include "EnemyRandomValues.h"

EnemyRandomValues::EnemyRandomValues(int minHp, int maxHp, int minAtck, int maxAtck, int minDfns, int maxDfns, int minExp, int maxExp) :
health(minHp,maxHp), attack(minAtck,maxAtck), defense(minDfns,maxDfns), exp(minExp,maxExp)
{
	
}

EnemyRandomValues::~EnemyRandomValues()
{
}
