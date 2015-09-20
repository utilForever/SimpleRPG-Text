#include "Dialogue.h"

Dialogue::Dialogue(std::string _description, std::vector<std::string> _choices)
	: description(_description), choices(_choices)
{
}

int Dialogue::activate()
{
	std::cout << description << std::endl;

	for (auto& choice : choices)
	{
		std::cout << choice << std::endl;
	}

	int userInput = -1;

	while (true)
	{
		std::cin >> userInput;

		if (userInput >= 0 && userInput <= choices.size())
		{
			return userInput;
		}
	}

	return 0;
}