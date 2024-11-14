#include "DifficultyManager.h"

EnemyRandomValues DifficultyManager::GetRandomValues(int dif) 
{
	return difficultyValues[dif];
}
EnemyRandomValues DifficultyManager::difficultyValues[DIFFICULTIES] = {
	EnemyRandomValues(1,2,  1,2,  1,2, 3,6),
	EnemyRandomValues(4,9,  2,3,  1,2, 6,15),
	EnemyRandomValues(5,10,  2,4,  1,3, 8,19),
	EnemyRandomValues(9,17,  1,7,  1,4, 17,25),
	EnemyRandomValues(9,17,  1,7,  1,4, 17,25),

};