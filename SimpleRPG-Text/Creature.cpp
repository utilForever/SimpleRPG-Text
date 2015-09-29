#include <string>
#include <cstdlib>

#include "JsonBox.h"

#include "Creature.h"
#include "Entity.h"
#include "Inventory.h"
#include "Weapon.h"
#include "Armor.h"
#include "Door.h"
#include "Area.h"
#include "EntityManager.h"

Creature::Creature(std::string _id, std::string _name, int _hp, int _strength, int _agility, double _evasion, unsigned _xp)
	: Entity(_id), name(_name), hp(_hp), maxHP(_hp), strength(_strength), agility(_agility), evasion(_evasion), xp(_xp), inventory(new Inventory()), equippedWeapon(nullptr), equippedArmor(nullptr)
{

}

Creature::Creature(std::string _id, JsonBox::Value& _v, EntityManager* _manager)
	: Entity(_id)
{
	Load(_v, _manager);
}

std::string Creature::GetName()
{
	return name;
}

int Creature::GetHP()
{
	return hp;
}

void Creature::SetHP(int _hp)
{
	hp = _hp;
}

int Creature::GetMaxHP()
{
	return maxHP;
}

void Creature::SetMaxHP(int _maxHP)
{
	maxHP = _maxHP;
}

int Creature::GetStrength()
{
	return strength;
}

void Creature::SetStrength(int _strength)
{
	strength = _strength;
}

int Creature::GetAgility()
{
	return agility;
}

void Creature::SetAgility(int _agility)
{
	agility = _agility;
}

unsigned int Creature::GetXP()
{
	return xp;
}

Inventory* Creature::GetInventory()
{
	return inventory;
}

Weapon* Creature::GetEquippedWeapon()
{
	return equippedWeapon;
}

Armor* Creature::GetEquippedArmor()
{
	return equippedArmor;
}

std::string Creature::GetCurrentArea()
{
	return currentArea;
}

void Creature::SetCurrentArea(std::string _currentArea)
{
	currentArea = _currentArea;
}

void Creature::EquipWeapon(Weapon* weapon)
{
	equippedWeapon = weapon;
}

void Creature::EquipArmor(Armor* armor)
{
	equippedArmor = armor;
}

Area* Creature::GetAreaPointer(EntityManager* manager)
{
	return manager->GetEntity<Area>(currentArea);
}

int Creature::Attack(Creature* target)
{
	int damage = 0;

	if (static_cast<double>(std::rand()) / RAND_MAX > target->evasion)
	{
		int attack = strength + ((equippedWeapon == nullptr) ? 0 : equippedWeapon->GetDamage());
		int defense = agility + ((equippedArmor == nullptr) ? 0 : equippedArmor->GetDefense());

		if (std::rand() % 32 == 0)
		{
			damage = attack / 2 + std::rand() % (attack / 2 + 1);
		}
		else
		{
			int baseDamage = attack - defense / 2;
			damage = baseDamage / 4 + std::rand() % (baseDamage / 4 + 1);
			if (damage < 1)
				damage = std::rand() % 2;
		}

		target->hp -= damage;
	}

	return damage;
}

int Creature::Traverse(Door* door)
{
	int flag = 2;

	if (door->GetLocked() == 0)
	{
		door->SetLocked(-1);
		flag = 2;
	}
	else if (door->GetLocked() > 0)
	{
		if (inventory->Count(door->GetKey()))
		{
			door->SetLocked(-1);
			flag = 1;
		}
		else
		{
			return 0;
		}
	}

	if (door->GetArea().first == currentArea)
		currentArea = door->GetArea().second;
	else if (door->GetArea().second == currentArea)
		currentArea = door->GetArea().first;

	return flag;
}

JsonBox::Object Creature::ToJSON()
{
	JsonBox::Object obj;

	obj["name"] = JsonBox::Value(name);
	obj["hp"] = JsonBox::Value(hp);
	obj["hp_max"] = JsonBox::Value(maxHP);
	obj["strength"] = JsonBox::Value(strength);
	obj["agility"] = JsonBox::Value(agility);
	obj["evasion"] = JsonBox::Value(evasion);
	obj["xp"] = JsonBox::Value(static_cast<int>(xp));
	obj["inventory"] = JsonBox::Value(inventory->GetJSON());
	obj["equipped_weapon"] = JsonBox::Value(equippedWeapon == nullptr ? "nullptr" : equippedWeapon->GetID());
	obj["equipped_armor"] = JsonBox::Value(equippedArmor == nullptr ? "nullptr" : equippedArmor->GetID());

	return obj;
}

void Creature::Load(JsonBox::Value& v, EntityManager* manager)
{
	JsonBox::Object obj = v.getObject();

	name = obj["name"].getString();
	hp = obj["hp"].getInteger();

	if (obj.find("hp_max") != obj.end())
		maxHP = obj["hp_max"].getInteger();
	else
		maxHP = hp;

	strength = obj["strength"].getInteger();
	agility = obj["agility"].getInteger();
	evasion = obj["evasion"].getDouble();

	if (obj.find("inventory") != obj.end())
		inventory = new Inventory(obj["inventory"], manager);

	if (obj.find("equipped_weapon") != obj.end())
	{
		std::string equippedWeaponName = obj["equipped_weapon"].getString();
		equippedWeapon = equippedWeaponName == "nullptr" ? nullptr : manager->GetEntity<Weapon>(equippedWeaponName);
	}

	if (obj.find("equipped_armor") != obj.end())
	{
		std::string equippedArmorName = obj["equipped_armor"].getString();
		equippedArmor = equippedArmorName == "nullptr" ? nullptr : manager->GetEntity<Armor>(equippedArmorName);
	}
}