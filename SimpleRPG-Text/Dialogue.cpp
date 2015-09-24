#include "Dialogue.h"

Dialogue::Dialogue()
{
	
}

Dialogue::Dialogue(std::string _description, std::vector<std::string> _choices)
	: description(_description), choices(_choices)
{
	
}

Dialogue::Dialogue(JsonBox::Value& _v)
{
	JsonBox::Object obj = _v.getObject();

	description = obj["Description"].getString();
	for (auto& choice : obj["Choices"].getArray())
		choices.push_back(choice.getString());
}

void Dialogue::AddChoice(std::string choice)
{
	choices.push_back(choice);
}

unsigned int Dialogue::Size()
{
	return choices.size();
}

int Dialogue::Activate()
{
	std::cout << description << std::endl;

	unsigned int number = 1;
	for (auto& choice : choices)
		std::cout << number++ << ": " << choice << std::endl;

	unsigned int userInput = 0;
	while (true)
	{
		std::cin >> userInput;
		if (userInput >= 1 && userInput <= choices.size())
			return userInput;
	}
}