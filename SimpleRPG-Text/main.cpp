#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <list>
#include <map>

#include "JsonBox.h"

#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
#include "Inventory.h"
#include "Creature.h"
#include "Player.h"
#include "Dialogue.h"
#include "Area.h"
#include "Door.h"
#include "Battle.h"
#include "EntityManager.h"


#define MAX_PLAYER_NAME_SIZE 14

Player StartGame();

void DialogueMenu(Player& player);

EntityManager entityManager;

int main(int argc, char* argv[])
{
	entityManager.LoadJSON<Item>("items.json");
	entityManager.LoadJSON<Weapon>("weapons.json");
	entityManager.LoadJSON<Armor>("armor.json");
	entityManager.LoadJSON<Creature>("creatures.json");
	entityManager.LoadJSON<Door>("doors.json");
	entityManager.LoadJSON<Area>("areas.json");

	std::srand(std::time(nullptr));

	Player player = StartGame();
	player.SetCurrentArea("area_01");

	while (1)
	{
		player.GetVisitedArea().insert(player.GetCurrentArea());

		Area* areaPointer = player.GetAreaPointer(&entityManager);

		player.Save(&entityManager);

		if (areaPointer->GetCreature().size() > 0)
		{
			std::vector<Creature*> combatants;
			std::cout << "You are attacked by ";
			for (int i = 0; i < areaPointer->GetCreature().size(); ++i)
			{
				Creature* c = &(areaPointer->GetCreature().at(i));
				combatants.push_back(c);
				std::cout << c->GetName() << (i == areaPointer->GetCreature().size() - 1 ? "!\n" : ", ");
			}
			combatants.push_back(&player);

			Battle battle(combatants);
			battle.Run();

			areaPointer->GetCreature().clear();
		}

		if (player.GetHP() <= 0)
		{
			std::cout << "\t----YOU DIED----\n    Game Over\n";
			return 0;
		}

		Dialogue roomOptions = areaPointer->GetDialogue();
		for (auto door : areaPointer->GetDoor())
		{
			roomOptions.AddChoice("Go through the " + door->GetDescription());
		}
		roomOptions.AddChoice("Search");

		int result = roomOptions.Activate();

		if (result == 0)
		{
			DialogueMenu(player);
		}
		else if (result <= areaPointer->GetDialogue().Size())
		{
			// Add more events here
		}
		else if (result < roomOptions.Size())
		{
			Door* door = areaPointer->GetDoor().at(result - areaPointer->GetDialogue().Size() - 1);
			int flag = player.Traverse(door);

			switch (flag)
			{
			default:
			case 0:
				std::cout << "The " << door->GetDescription() << " is locked." << std::endl;
				break;
			case 1:
				std::cout << "You unlock the " << door->GetDescription() << " and go through it." << std::endl;
				break;
			case 2:
				std::cout << "You go through the " << door->GetDescription() << "." << std::endl;
				break;
			}
		}
		else
		{
			std::cout << "You find:" << std::endl;
			areaPointer->GetItem().Print();
			player.GetInventory()->Merge(&(areaPointer->GetItem()));
			areaPointer->GetItem().Clear();
		}
	}

	return 0;
}

Player StartGame()
{
	std::cout << "What's your name?" << std::endl;
    std::string name = "";
	std::getline(std::cin, name);

	std::ifstream f((name + ".json").c_str());
	if (f.good())
	{
		f.close();

		JsonBox::Value saveData;
		JsonBox::Value areaData;
		saveData.loadFromFile(name + ".json");
		areaData.loadFromFile(name + "_areas.json");

		Player player = Player(saveData, areaData, &entityManager);

		return player;
	}
	else
	{
		f.close();

		int result = Dialogue("Choose your class", { "Fighter", "Rogue" }).Activate();
		switch (result)
		{
		case 1:
			return Player(name, 15, 5, 4, 1.0 / 64.0, 0, 1, "Fighter");
		case 2:
			return Player(name, 15, 4, 5, 1.0 / 64.0, 0, 1, "Rogue");
		default:
			return Player(name, 15, 4, 4, 1.0 / 64.0, 0, 1, "Adventurer");
		}
	}
}

void DialogueMenu(Player& player)
{
	int result = Dialogue("Menu\n====", { "Items", "Equipment", "Character" }).Activate();

	switch (result)
	{
	case 1:
		std::cout << "Items\n=====\n";
		player.GetInventory()->Print();
		std::cout << "----------------\n";
		break;
	case 2:
	{
		std::cout << "Equipment\n=========\n";
		std::cout << "Armor: " << (player.GetEquippedArmor() != nullptr ? player.GetEquippedArmor()->GetName() : "Nothing") << std::endl;
		std::cout << "Weapon: " << (player.GetEquippedWeapon() != nullptr ? player.GetEquippedWeapon()->GetName() : "Nothing") << std::endl;

		int result2 = Dialogue("", { "Equip Armor", "Equip Weapon", "Close" }).Activate();

		if (result2 == 1)
		{
			int userInput = 0;

			int numItems = player.GetInventory()->Print<Armor>(true);
			if (numItems == 0) break;

			while (!userInput)
			{
				std::cout << "Equip which item?" << std::endl;
				std::cin >> userInput;
				if (userInput >= 1 && userInput <= numItems)
				{
					player.EquipArmor(player.GetInventory()->Get<Armor>(userInput - 1));
				}
			}
		}
		else if (result2 == 2)
		{
			int userInput = 0;
			int numItems = player.GetInventory()->Print<Weapon>(true);

			if (numItems == 0) break;

			while (!userInput)
			{
				std::cout << "Equip which item?" << std::endl;
				std::cin >> userInput;
				if (userInput >= 1 && userInput <= numItems)
				{
					player.EquipWeapon(player.GetInventory()->Get<Weapon>(userInput - 1));
				}
			}
		}
		std::cout << "----------------\n";
		break;
	}
	case 3:
		std::cout << "Character\n=========\n";
		std::cout << player.GetName();
		if (player.GetClassName() != "") std::cout << " the " << player.GetClassName();
		std::cout << std::endl;

		std::cout << "Health:   " << player.GetHP() << " / " << player.GetMaxHP() << std::endl;
		std::cout << "Strength: " << player.GetStrength() << std::endl;
		std::cout << "Agility:  " << player.GetAgility() << std::endl;
		std::cout << "Level:    " << player.GetLevel() << " (" << player.GetXP();
		std::cout << " / " << player.GetXPToLevelUp(player.GetLevel() + 1) << ")" << std::endl;
		std::cout << "----------------\n";
		break;
	default:
		break;
	}
}