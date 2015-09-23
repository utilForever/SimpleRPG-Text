#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "JsonBox.h"

class EntityManager;

class Entity
{
private:
	std::string id;

public:
	Entity(std::string _id);
	virtual ~Entity();

	virtual void Load(JsonBox::Value& v, EntityManager* manager) = 0;
};

#endif