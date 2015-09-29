#ifndef DOOR_H
#define DOOR_H

#include <string>
#include <utility>

#include "Entity.h"

class Item;
class EntityManager;

class Door : public Entity
{
private:
	std::string description;
	int locked;
	Item* key;
	std::pair<std::string, std::string> areas;

public:
	Door(std::string _id, std::string _description, std::pair<std::string, std::string> _areas, int _locked, Item* _key = nullptr);
	Door(std::string _id, JsonBox::Value& _v, EntityManager* _manager);

	std::string GetDescription();
	int GetLocked();
	void SetLocked(int _locked);

	Item* GetKey();

	std::pair<std::string, std::string> GetArea();
	void SetArea(std::pair<std::string, std::string> _areas);

	void Load(JsonBox::Value& v, EntityManager* manager);
};

#endif