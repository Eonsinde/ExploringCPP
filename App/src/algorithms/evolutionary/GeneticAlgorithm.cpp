#include "EntryPoint/Application.h"

#include <iostream>
#include <format>
#include <string>

// This example implements the Genetic Algorithm which is a search based
// optimization technique based off Darwin's theory of evolution(Survival of the fittest)
class GAApplication : Core::Application
{
public:
	virtual void Run() override {
		std::string name = "Enoch";
		std::string test = std::format("This is the way {} said it's done", name);

		std::cout << test << '\n';
	}
};

DECLARE_MAIN(GAApplication)

