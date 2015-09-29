#ifndef ARMOR_H
#define ARMOR_H

#include <string>

#include "JsonBox.h"

#include "Item.h"

class EntityManager;

class Armor : public Item
{
private:
	int defense;

public:
	Armor(std::string _id, std::string _name, std::string _description, int _defense);
	Armor(std::string _id, JsonBox::Value& _v, EntityManager* _manager);

	int GetDefense();

	void Load(JsonBox::Value& v, EntityManager* manager) override;
};

#endif