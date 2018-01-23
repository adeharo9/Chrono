#ifndef CHRONO_TEST_H
#define CHRONO_TEST_H

#include <vector>

using namespace std;

class Test
{
	public:

		Test() = default;

		virtual void initialize() = 0;

		virtual void run() = 0;

		virtual void dataProcessing() = 0;

		virtual void end() = 0;
};


#endif //CHRONO_TEST_H
