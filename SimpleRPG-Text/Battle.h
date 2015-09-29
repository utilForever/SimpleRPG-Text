#ifndef BATTLE_H
#define BATTLE_H

#include <vector>

#include "Dialogue.h"

class Creature;

enum class BattleEventType { ATTACK, DEFEND };

class BattleEvent
{
private:
	Creature* source;
	Creature* target;
	BattleEventType type;

public:
	BattleEvent(Creature* _source, Creature* _target, BattleEventType _type);

	Creature* GetSource();
	Creature* GetTarget();
	BattleEventType GetType();
	
	int Run();
};

class Battle
{
private:
	std::vector<Creature*> combatants;
	Dialogue battleOptions;

public:
	Battle(std::vector<Creature*>& _combatants);

	void Run();
	void Kill(Creature* creature);
	void NextTurn();
};

#endif