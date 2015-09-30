#ifndef PLAYER_H
#define PLAYER_H

#include <unordered_set>
#include <string>

#include "JsonBox.h"

#include "Creature.h"

class EntityManager;

class Player : public Creature
{
private:
	std::string className;
	unsigned int level;
	std::unordered_set<std::string> visitedAreas;

public:
	Player();
	Player(std::string _name, int _hp, int _strength, int _agility, double _evasion, unsigned int _xp, unsigned int _level, std::string _className);
	Player(JsonBox::Value& _saveData, JsonBox::Value& _areaData, EntityManager* _manager);

	std::string GetClassName();
	unsigned int GetLevel();
	std::unordered_set<std::string>& GetVisitedArea();

	unsigned int GetXPToLevelUp(unsigned int level);
	bool LevelUp();

	JsonBox::Object ToJSON();

	void Save(EntityManager* manager);
	void Load(JsonBox::Value& saveData, EntityManager* manager);
	void LoadArea(JsonBox::Value& areaData, EntityManager* manager);
};

#endif