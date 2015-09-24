#ifndef INVENTORY_H
#define INVENTORY_H

#include <list>
#include <utility>

#include "JsonBox.h"

#include "EntityManager.h"

class Item;
class Weapon;
class Armor;

class Inventory
{
private:
	std::list<std::pair<Item*, int>> items;

	template <typename T>
	void Load(JsonBox::Value& v, EntityManager* manager);

	template <typename T>
	JsonBox::Array JSONArray();

public:
	Inventory();
	Inventory(JsonBox::Value& _v, EntityManager* _manager);
	
	void Add(Item* itemToAdd, int count);
	void Remove(Item* itemToRemove, int count);

	int Count(Item* itemToCount);
	template <typename T>
	int Count(unsigned int pos);

	template <typename T>
	T* Get(unsigned int pos);

	int Print(bool label = false);
	template <typename T>
	int Print(bool label = false);

	void Clear();

	void Merge(Inventory* inventory);

	JsonBox::Object GetJSON();
};

#endif