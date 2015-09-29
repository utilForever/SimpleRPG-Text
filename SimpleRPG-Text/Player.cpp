#include <unordered_set>
#include <cmath>

#include "JsonBox.h"

#include "Area.h"
#include "Player.h"
#include "Creature.h"
#include "EntityManager.h"

Player::Player()
	: Player("", 0, 0, 0, 0.0, 0, 1, "nullID")
{
	
}

Player::Player(std::string _name, int _hp, int _strength, int _agility, double _evasion, unsigned _xp, unsigned _level, std::string _className)
	: Creature("Player", _name, _hp, _strength, _agility, _evasion, _xp), className(_className), level(_level)
{
	
}

Player::Player(JsonBox::Value& _saveData, JsonBox::Value& _areaData, EntityManager* _manager)
	: Player()
{
	Load(_saveData, _manager);
	LoadArea(_areaData, _manager);
}

std::string Player::GetClassName()
{
	return className;
}

unsigned int Player::GetLevel()
{
	return level;
}

std::unordered_set<std::string> Player::GetVisitedArea()
{
	return visitedAreas;
}

unsigned int Player::GetXPToLevelUp(unsigned int level)
{
	return static_cast<unsigned int>(1.5 * std::pow(level, 3));
}

bool Player::LevelUp()
{
	if (GetXP() < GetXPToLevelUp(level + 1))
		return false;

	level++;

	unsigned int statIncreases[3] = { 0, 0, 0 };
	float statMultipliers[3] = { 0, 0, 0 };
	statMultipliers[0] = 13.0f;
	statMultipliers[1] = className == "Fighter" ? 8.0f : 6.0f;
	statMultipliers[2] = className == "Rogue" ? 8.0f : 6.0f;

	for (int i = 0; i < 3; i++)
	{
		float base = std::tanh(level / 30.0) * ((level % 2) + 1);
		statIncreases[i] += static_cast<int>(1 + statMultipliers[i] * base);
	}

	SetHP(GetHP() + statIncreases[0]);
	SetMaxHP(GetMaxHP() + statIncreases[0]);
	SetStrength(GetStrength() + statIncreases[1]);
	SetAgility(GetAgility() + statIncreases[2]);

	std::cout << GetName() << " grew to level " << level << "!\n";
	std::cout << "Health   +" << statIncreases[0] << " -> " << GetMaxHP() << std::endl;
	std::cout << "Strength +" << statIncreases[1] << " -> " << GetStrength() << std::endl;
	std::cout << "Agility  +" << statIncreases[2] << " -> " << GetAgility() << std::endl;
	std::cout << "----------------\n";

	return true;
}

JsonBox::Object Player::ToJSON()
{
	JsonBox::Object obj = Creature::ToJSON();

	obj["className"] = JsonBox::Value(className);
	obj["level"] = JsonBox::Value(static_cast<int>(level));

	return obj;
}

void Player::Save(EntityManager* manager)
{
	JsonBox::Value v(ToJSON());
	v.writeToFile(GetName() + ".json");

	JsonBox::Object obj;
	for (auto area : visitedAreas)
		obj[area] = manager->GetEntity<Area>(area)->GetJSON();

	JsonBox::Value v2(obj);
	v2.writeToFile(GetName() + "_areas.json");
}

void Player::Load(JsonBox::Value& saveData, EntityManager* manager)
{
	Creature::Load(saveData, manager);
	JsonBox::Object obj = saveData.getObject();

	className = obj["CclassName"].getString();
	level = obj["level"].getInteger();
}

void Player::LoadArea(JsonBox::Value& areaData, EntityManager* manager)
{
	JsonBox::Object obj = areaData.getObject();
	for (auto area : obj)
	{
		std::string key = area.first;
		manager->GetEntity<Area>(key)->Load(area.second, manager);
		visitedAreas.insert(key);
	}
}