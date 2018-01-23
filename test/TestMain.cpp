#include <iostream>
#include <vector>
#include "test-classes/Test.h"
#include "test-classes/ModeTest.h"

int main()
{
	vector<Test*> tests = vector<Test*>({new ModeTest()});

	for (Test* test : tests)
	{
		test -> initialize();
		test -> run();
		test -> dataProcessing();
		test -> end();
	}

	return 0;
}
