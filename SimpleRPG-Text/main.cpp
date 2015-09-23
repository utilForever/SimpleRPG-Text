#include "JsonBox.h"

#include "Item.h"
#include "Armor.h"
#include "Weapon.h"
#include "EntityManager.h"

EntityManager entityManager;

int main(int argc, char* argv[])
{
	entityManager.LoadJSON<Item>("items.json");
	entityManager.LoadJSON<Weapon>("weapons.json");
	entityManager.LoadJSON<Armor>("armor.json");

	return 0;
}