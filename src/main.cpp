#include "TestEnv.hpp"
#include <iostream>
int main()
{
    std::cout << "Start!\n";
	TestEnv* t = new TestEnv();

	t->run();
	
	delete t;
	char waitForExit;
	std::cin >> waitForExit;
}

