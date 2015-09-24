#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <vector>
#include <iostream>

#include "JsonBox.h"

class Dialogue
{
private:
	std::string description;
	std::vector<std::string> choices;

public:
	Dialogue();
	Dialogue(std::string _description, std::vector<std::string> _choices);
	Dialogue(JsonBox::Value& _v);

	void AddChoice(std::string choice);
	unsigned int Size();

	int Activate();
};

#endif