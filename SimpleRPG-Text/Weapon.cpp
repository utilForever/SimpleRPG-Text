#include <string>

#include "JsonBox.h"

#include "Weapon.h"
#include "Item.h"
#include "EntityManager.h"

Weapon::Weapon(std::string _id, std::string _name, std::string _description, int _damage)
	: Item(_id, _name, _description), damage(_damage)
{

}

Weapon::Weapon(std::string _id, JsonBox::Value& _v, EntityManager* _manager)
	: Item(_id, _v, _manager)
{
	Load(_v, _manager);
}

int Weapon::GetDamage()
{
	return damage;
}

void Weapon::Load(JsonBox::Value& v, EntityManager* manager)
{
	JsonBox::Object obj = v.getObject();

	damage = obj["damage"].getInteger();
}
