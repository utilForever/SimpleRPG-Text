#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <string>
#include <map>

#include "Entity.h"

class EntityManager
{
private:
	std::map<std::string, Entity*> data;

public:
	EntityManager();
	~EntityManager();

	template <typename T>
	void LoadJSON(std::string fileName);

	template <typename T>
	T* GetEntity(std::string id);
};

template <typename T>
std::string EntityToString();

#endif