#pragma once
#include "BattleState.h"
#include "character.hpp"
#include <string>
#include <tuple>

class BattleDataManager
{
public:
	BattleDataManager();

	std::tuple<bool, BattleState> LoadBattleFile();
	void SaveData() const;
	void NewBattleFile(const Character& player, const int difficulty) const;
	void RegisterData(const BattleState state);
private:
	std::string serializedData;
	const std::string dataPath;
};

