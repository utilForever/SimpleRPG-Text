#include "Entity.h"

Entity::Entity(std::string _id)
	: id(_id)
{
	
}

Entity::~Entity()
{
	
}

std::string Entity::GetID()
{
	return id;
}