#include <string>

#include "JsonBox.h"

#include "Armor.h"
#include "Item.h"
#include "EntityManager.h"

Armor::Armor(std::string _id, std::string _name, std::string _description, int _defense)
	: Item(_id, _name, _description), defense(_defense)
{
	
}

Armor::Armor(std::string _id, JsonBox::Value& _v, EntityManager* _manager)
	: Item(_id, _v, _manager)
{
	Load(_v, _manager);
}

void Armor::Load(JsonBox::Value& v, EntityManager* manager)
{
	JsonBox::Object obj = v.getObject();
	
	defense = obj["defense"].getInteger();
}
