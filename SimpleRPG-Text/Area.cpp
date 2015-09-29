#include <vector>
#include <string>

#include "JsonBox.h"

#include "Area.h"
#include "Door.h"
#include "Entity.h"
#include "Inventory.h"
#include "Dialogue.h"
#include "EntityManager.h"

Area::Area(std::string _id, Dialogue _dialogue, Inventory _items, std::vector<Creature*> _creatures)
	: Entity(_id), dialogue(_dialogue), items(_items)
{
	for (auto creature : _creatures)
		creatures.push_back(*creature);
}

Area::Area(std::string _id, JsonBox::Value& _v, EntityManager* _manager)
	: Entity(_id)
{
	Load(_v, _manager);
}

Dialogue Area::GetDialogue()
{
	return dialogue;
}

Inventory& Area::GetItem()
{
	return items;
}

std::vector<Door*> Area::GetDoor()
{
	return doors;
}

std::vector<Creature>& Area::GetCreature()
{
	return creatures;
}

void Area::Load(JsonBox::Value& v, EntityManager* manager)
{
	JsonBox::Object obj = v.getObject();

	if (obj.find("dialogue") != obj.end())
		dialogue = Dialogue(obj["dialogue"]);

	items = Inventory(obj["inventory"], manager);

	creatures.clear();
	for (auto creature : obj["creatures"].getArray())
	{
		Creature c(*manager->GetEntity<Creature>(creature.getString()));
		creatures.push_back(c);
	}

	if (obj.find("doors") != obj.end())
	{
		doors.clear();

		for (auto door : obj["doors"].getArray())
		{
			Door* d = nullptr;

			if (door.isString())
			{
				d = manager->GetEntity<Door>(door.getString());
			}
			else
			{
				d = manager->GetEntity<Door>(door.getArray()[0].getString());
				d->SetLocked(door.getArray()[1].getInteger());
			}

			doors.push_back(d);
		}
	}
}

JsonBox::Object Area::GetJSON()
{
	JsonBox::Object obj;

	obj["inventory"] = items.GetJSON();

	JsonBox::Array arr;
	for (auto door : doors)
	{
		JsonBox::Array d;
		d.push_back(door->GetID());
		d.push_back(door->GetLocked());
		
		arr.push_back(d);
	}

	obj["doors"] = arr;

	return obj;
}