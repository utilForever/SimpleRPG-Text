#include <string>
#include <utility>

#include "Door.h"
#include "Item.h"
#include "Entity.h"
#include "EntityManager.h"

Door::Door(std::string _id, std::string _description, std::pair<std::string, std::string> _areas, int _locked, Item* _key)
	: Entity(_id), description(_description), areas(_areas), locked(_locked), key(_key)
{
	
}

Door::Door(std::string _id, JsonBox::Value& _v, EntityManager* _manager)
	: Entity(_id)
{
	Load(_v, _manager);
}

std::string Door::GetDescription()
{
	return description;
}

int Door::GetLocked()
{
	return locked;
}

void Door::SetLocked(int _locked)
{
	locked = _locked;
}

Item* Door::GetKey()
{
	return key;
}

std::pair<std::string, std::string> Door::GetArea()
{
	return areas;
}

void Door::SetArea(std::pair<std::string, std::string> _areas)
{
	areas = _areas;
}

void Door::Load(JsonBox::Value& v, EntityManager* manager)
{
	JsonBox::Object obj = v.getObject();

	description = obj["description"].getString();
	locked = obj["locked"].getInteger();

	if (obj.find("key") != obj.end())
		key = manager->GetEntity<Item>(obj["key"].getString());

	JsonBox::Array arr = obj["areas"].getArray();

	if (arr.size() == 2)
	{
		areas.first = arr[0].getString();
		areas.second = arr[1].getString();
	}
}