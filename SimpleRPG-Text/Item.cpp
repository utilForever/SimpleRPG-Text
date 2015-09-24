#include <string>

#include "JsonBox.h"

#include "Item.h"
#include "Entity.h"
#include "EntityManager.h"

Item::Item(std::string _id, std::string _name, std::string _description)
	: Entity(_id), name(_name), description(_description)
{
	
}

Item::Item(std::string _id, JsonBox::Value& _v, EntityManager* _manager)
	: Entity(_id)
{
	Load(_v, _manager);
}

std::string Item::GetName() const
{
	return name;
}

std::string Item::GetDescription() const
{
	return description;
}

void Item::Load(JsonBox::Value& v, EntityManager* manager)
{
	JsonBox::Object obj = v.getObject();
	
	name = obj["name"].getString();
	description = obj["description"].getString();
}