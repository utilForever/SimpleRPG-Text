#ifndef CREATURE_H
#define CREATURE_H

#include <string>

class Creature
{
private:
	std::string name;
	std::string className;

	int health;
	int maxHealth;

	int str;
	int dex;

	double hitRate;

	unsigned int level;
	unsigned int exp;
public:
	Creature();
	Creature(std::string _name, std::string _className, int _health, int _str, int _dex, double _hitRate, unsigned int _level);

	unsigned int getRequiredExpToNextLevel(unsigned int level);
	bool levelUp();
};

#endif