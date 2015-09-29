#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include "Battle.h"
#include "Creature.h"
#include "Dialogue.h"

BattleEvent::BattleEvent(Creature* _source, Creature* _target, BattleEventType _type)
	: source(_source), target(_target), type(_type)
{
	
}

Creature* BattleEvent::GetSource()
{
	return source;
}

Creature* BattleEvent::GetTarget()
{
	return target;
}

BattleEventType BattleEvent::GetType()
{
	return type;
}

int BattleEvent::Run()
{
	switch (type)
	{
	case BattleEventType::ATTACK:
		return source->Attack(target);
	case BattleEventType::DEFEND:
		return 0;
	default:
		return 0;
	}
}

Battle::Battle(std::vector<Creature*>& _combatants)
	: combatants(_combatants)
{
	battleOptions = Dialogue("What will you do?", { "Attack", "Defend" });

	std::map<std::string, int> names;
	for (auto com : combatants)
	{
		if (com->GetID() == "Player")
			continue;

		if (names.count(com->GetName()) == 0)
			names[com->GetName()] = 0;
		else if (names[com->GetName()] == 0)
			names[com->GetName()] = 1;
	}

	for (auto& com : combatants)
	{
		std::string newName = com->GetName();

		if (names.count(com->GetName()) > 0 && names[com->GetName()] > 0)
		{
			newName += " (" + std::to_string(names[com->GetName()]) + ")";
			names[com->GetName()] += 1;
		}

		com->GetName() = newName;
	}
}

void Battle::Run()
{
	// TODO : check player != end
	std::vector<Creature*>::iterator player;
	std::vector<Creature*>::iterator end;
	do {
		player = std::find_if(combatants.begin(), combatants.end(),
			[](Creature* creature) { return creature->GetID() == "Player"; });
		end = combatants.end();

		NextTurn();
	} while (combatants.size() > 1 && player != end);
}

void Battle::Kill(Creature* creature)
{
	auto pos = std::find(combatants.begin(), combatants.end(), creature);

	if (pos != combatants.end())
	{
		std::cout << creature->GetName() << " is slain!" << std::endl;
		creature->SetHP(0);
		combatants.erase(pos);
	}
}

void Battle::NextTurn()
{
	std::queue<BattleEvent> events;

	std::sort(combatants.begin(), combatants.end(),
		[](Creature* a, Creature* b) { return a->GetAgility() > b->GetAgility(); });

	for (auto com : combatants)
	{
		if (com->GetID() == "Player")
		{
			Dialogue targetSelection = Dialogue("Who?", {});
			for (auto target : combatants)
				if (target->GetID() != "Player")
					targetSelection.AddChoice(target->GetName());

			int choice = battleOptions.Activate();

			switch (choice)
			{
			default:
			case 1:
			{
				int position = targetSelection.Activate();
				for (int i = 0; i < position; i++)
					if (combatants[i]->GetID() == "Player")
						position++;

				Creature* target = combatants[position - 1];
				events.push(BattleEvent(com, target, BattleEventType::ATTACK));
				break;
			}
			case 2:
			{
				events.push(BattleEvent(com, nullptr, BattleEventType::DEFEND));
				break;
			}
			}
		}
		else
		{
			Creature* player = *std::find_if(combatants.begin(), combatants.end(),
				[](Creature* a) { return a->GetID() == "Player"; });
			events.push(BattleEvent(com, player, BattleEventType::ATTACK));
		}
	}

	while (!events.empty())
	{
		BattleEvent event = events.front();
		switch (event.GetType())
		{
		case BattleEventType::ATTACK:
		{
			auto a = combatants.begin();
			auto b = combatants.end();

			if (std::find(a, b, event.GetSource()) == b || std::find(a, b, event.GetTarget()) == b)
				break;

			std::cout << event.GetSource()->GetName() << " attacks " << event.GetTarget()->GetName() << " for "	<< event.Run() << " damage!" << std::endl;
			if (event.GetTarget()->GetHP() <= 0)
				Kill(event.GetTarget());

			break;
		}
		case BattleEventType::DEFEND:
			std::cout << event.GetSource()->GetName() << " defends!\n";
			break;
		default:
			break;
		}

		events.pop();
	}
}