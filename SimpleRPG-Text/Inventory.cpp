#include <list>
#include <string>
#include <utility>
#include <iostream>

#include "JsonBox.h"

#include "Inventory.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "EntityManager.h"

template <typename T>
void Inventory::Load(JsonBox::Value& v, EntityManager* manager)
{
	for (auto item : v.getArray())
	{
		std::string itemID = item.getArray()[0].getString();
		int quantity = item.getArray()[1].getInteger();
		items.push_back(std::make_pair(manager->GetEntity<T>(itemID), quantity));
	}
}

template <typename T>
JsonBox::Array Inventory::JSONArray()
{
	JsonBox::Array arr;

	for (auto item : items)
	{
		if (item.first->GetID().substr(0, EntityToString<T>().size()) != EntityToString<T>())
			continue;

		JsonBox::Array pair;
		pair.push_back(JsonBox::Value(item.first->GetID()));
		pair.push_back(JsonBox::Value(item.second));

		arr.push_back(JsonBox::Value(pair));
	}

	return arr;
}

Inventory::Inventory()
{
	
}

Inventory::Inventory(JsonBox::Value& _v, EntityManager* _manager)
{
	JsonBox::Object obj = _v.getObject();

	Load<Item>(obj["items"], _manager);
	Load<Weapon>(obj["weapons"], _manager);
	Load<Armor>(obj["armor"], _manager);
}

void Inventory::Add(Item* itemToAdd, int count)
{
	for (auto& item : items)
	{
		if (item.first->GetID() == itemToAdd->GetID())
		{
			item.second += count;
			return;
		}
	}

	items.push_back(std::make_pair(itemToAdd, count));
}

void Inventory::Remove(Item* itemToRemove, int count)
{
	for (auto item = items.begin(); item != items.end(); ++item)
	{
		if ((*item).first->GetID() == itemToRemove->GetID())
		{
			(*item).second -= count;
			
			if ((*item).second < 1)
				items.erase(item);

			return;
		}
	}
}

int Inventory::Count(Item* itemToCount)
{
	for (auto item : items)
	{
		if (item.first->GetID() == itemToCount->GetID())
			return item.second;
	}

	return 0;
}

template <typename T>
int Inventory::Count(unsigned int pos)
{
	return Count(Get<T>(pos));
}

template <typename T>
T* Inventory::Get(unsigned int pos)
{
	unsigned int i = 0;
	auto iter = items.begin();

	for (; iter != items.end(); ++iter)
	{
		if ((*iter).first->GetID().substr(0, EntityToString<T>().size()) != EntityToString<T>())
			continue;

		if (i++ == pos)
			break;
	}

	if (iter != items.end())
		return dynamic_cast<T*>((*iter).first);
	else
		return nullptr;
}

int Inventory::Print(bool label)
{
	unsigned int nItems = 0;

	if (items.empty())
	{
		std::cout << "Nothing" << std::endl;
	}
	else
	{
		nItems += Print<Item>(label);
		nItems += Print<Weapon>(label);
		nItems += Print<Armor>(label);
	}

	return nItems;
}

template <typename T>
int Inventory::Print(bool label)
{
	unsigned int number = 1;

	for (auto item : items)
	{
		if (item.first->GetID().substr(0, EntityToString<T>().size()) != EntityToString<T>())
			continue;

		if (label) std::cout << number++ << ": ";
		std::cout << item.first->GetName() << " (" << item.second << ") - ";
		std::cout << item.first->GetDescription() << std::endl;
	}

	return items.size();
}

void Inventory::Clear()
{
	items.clear();
}

void Inventory::Merge(Inventory* inventory)
{
	if (this == inventory)
		return;

	for (auto item : inventory->items)
		Add(item.first, item.second);
}

JsonBox::Object Inventory::GetJSON()
{
	JsonBox::Object obj;

	obj["items"] = JsonBox::Value(JSONArray<Item>());
	obj["weapons"] = JsonBox::Value(JSONArray<Weapon>());
	obj["armor"] = JsonBox::Value(JSONArray<Armor>());

	return obj;
}

template void Inventory::Load<Item>(JsonBox::Value&, EntityManager*);
template void Inventory::Load<Weapon>(JsonBox::Value&, EntityManager*);
template void Inventory::Load<Armor>(JsonBox::Value&, EntityManager*);

template JsonBox::Array Inventory::JSONArray<Item>();
template JsonBox::Array Inventory::JSONArray<Weapon>();
template JsonBox::Array Inventory::JSONArray<Armor>();

template int Inventory::Count<Item>(unsigned int);
template int Inventory::Count<Weapon>(unsigned int);
template int Inventory::Count<Armor>(unsigned int);

template Item* Inventory::Get<Item>(unsigned int);
template Weapon* Inventory::Get<Weapon>(unsigned int);
template Armor* Inventory::Get<Armor>(unsigned int);

template int Inventory::Print<Item>(bool);
template int Inventory::Print<Weapon>(bool);
template int Inventory::Print<Armor>(bool);