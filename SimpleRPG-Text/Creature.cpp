#include "Creature.h"

Creature::Creature()
{

}

Creature::Creature(std::string _name, std::string _className, int _health, int _str, int _dex, double _hitRate, unsigned int _level)
	: name(_name), className(_className), health(_health), maxHealth(_health), str(_str), dex(_dex), hitRate(_hitRate), level(_level), exp(0)
{

}

unsigned int Creature::getRequiredExpToNextLevel(unsigned int level)
{
	return 128 * level * level;
}

bool Creature::levelUp()
{
	if (this->exp >= getRequiredExpToNextLevel(this->level + 1))
	{
		level++;

		unsigned int healthBoost = 0;
		unsigned int strBoost = 0;
		unsigned int dexBoost = 0;

		healthBoost = 10 + (rand() % 4);

		if (this->className == "Fighter")
		{
			strBoost = 1;
			if (rand() % 2 == 0) dexBoost = 1;
		}
		else if (this->className == "Rogue")
		{
			dexBoost = 1;
			if (rand() % 2 == 0) strBoost = 1;
		}

		this->maxHealth += healthBoost;
		this->str += strBoost;
		this->dex += dexBoost;
	}
}