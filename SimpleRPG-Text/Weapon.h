#ifndef WEAPON_H
#define WEAPON_H

#include <string>

#include "JsonBox.h"

#include "Item.h"

class EntityManager;

class Weapon : public Item
{
private:
	int damage;

public:
	Weapon(std::string _id, std::string _name, std::string _description, int _damage);
	Weapon(std::string _id, JsonBox::Value& _v, EntityManager* _manager);

	int GetDamage();

	void Load(JsonBox::Value& v, EntityManager* manager) override;
};

#endif