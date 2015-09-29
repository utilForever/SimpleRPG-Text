#include <string>
#include <map>

#include "EntityManager.h"
#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Creature.h"
#include "Area.h"
#include "Door.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
	for (auto& entity : data)
	{
		if (entity.second != nullptr)
		{
			delete entity.second;
			entity.second = nullptr;
		}
	}
}

template <typename T>
void EntityManager::LoadJSON(std::string fileName)
{
	JsonBox::Value v;
	v.loadFromFile(fileName);

	JsonBox::Object obj = v.getObject();
	for (auto entity : obj)
	{
		std::string key = entity.first;
		data[key] = dynamic_cast<Entity*>(new T(key, entity.second, this));
	}
}

template <typename T>
T* EntityManager::GetEntity(std::string id)
{
	if (id.substr(0, EntityToString<T>().size()) == EntityToString<T>())
		return dynamic_cast<T*>(data.at(id));
	else
		return nullptr;
}

template <> std::string EntityToString<Item>() { return "item"; }
template <> std::string EntityToString<Weapon>() { return "weapon"; }
template <> std::string EntityToString<Armor>() { return "armor"; }
template <> std::string EntityToString<Creature>() { return "creature"; }
template <> std::string EntityToString<Area>() { return "area"; }
template <> std::string EntityToString<Door>() { return "door"; }

template void EntityManager::LoadJSON<Item>(std::string);
template void EntityManager::LoadJSON<Weapon>(std::string);
template void EntityManager::LoadJSON<Armor>(std::string);
template void EntityManager::LoadJSON<Creature>(std::string);
template void EntityManager::LoadJSON<Area>(std::string);
template void EntityManager::LoadJSON<Door>(std::string);

template Item* EntityManager::GetEntity<Item>(std::string);
template Weapon* EntityManager::GetEntity<Weapon>(std::string);
template Armor* EntityManager::GetEntity<Armor>(std::string);
template Creature* EntityManager::GetEntity<Creature>(std::string);
template Area* EntityManager::GetEntity<Area>(std::string);
template Door* EntityManager::GetEntity<Door>(std::string);
