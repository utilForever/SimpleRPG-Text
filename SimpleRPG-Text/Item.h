#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "JsonBox.h"

#include "Entity.h"

class EntityManager;

class Item : public Entity
{
private:
	std::string name;
	std::string description;

public:
	Item(std::string _id, std::string _name, std::string _description);
	Item(std::string _id, JsonBox::Value& _v, EntityManager* _manager);

	std::string GetName() const;
	std::string GetDescription() const;

	virtual void Load(JsonBox::Value& v, EntityManager* manager) override;
};

#endif