#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <cstdlib>

#include "JsonBox.h"

#include "Entity.h"
#include "Inventory.h"

class Area;
class EntityManager;
class Weapon;
class Armor;
class Door;

class Creature : public Entity
{
private:
	std::string name;

	int hp;
	int maxHP;
	int strength;
	int agility;
	double evasion;
	unsigned int xp;

	Inventory* inventory;

	Weapon* equippedWeapon;
	Armor* equippedArmor;

	std::string currentArea;

public:
	Creature(std::string _id, std::string _name, int _hp, int _strength, int _agility, double _evasion, unsigned int _xp);
	Creature(std::string _id, JsonBox::Value& _v, EntityManager* _manager);

	std::string GetName();

	int GetHP();
	void SetHP(int _hp);
	int GetMaxHP();
	void SetMaxHP(int _maxHP);
	int GetStrength();
	void SetStrength(int _strength);
	int GetAgility();
	void SetAgility(int _agility);

	unsigned int GetXP();

	Inventory* GetInventory();

	Weapon* GetEquippedWeapon();
	Armor* GetEquippedArmor();

	std::string GetCurrentArea();
	void SetCurrentArea(std::string _currentArea);

	void EquipWeapon(Weapon* weapon);
	void EquipArmor(Armor* armor);

	Area* GetAreaPointer(EntityManager* manager);

	int Attack(Creature* target);

	int Traverse(Door* door);

	virtual JsonBox::Object ToJSON();

	virtual void Load(JsonBox::Value& v, EntityManager* manager);
};

#endif