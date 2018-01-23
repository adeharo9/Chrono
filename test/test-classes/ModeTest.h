#ifndef CHRONO_MODETEST_H
#define CHRONO_MODETEST_H

#include "Test.h"
#include "TestState.h"
#include "../../src/Chrono.h"

typedef unsigned int uint;

class ModeTest : public Test
{
private:

	vector<Chrono::UsageMode> usageModes;

	TestState testState = INITIALIZE_STATE;
	Chrono::UsageMode usageMode;

public:
	ModeTest() = default;

	void initialize() override;

	void run() override;

	void dataProcessing() override;

	void end() override;

private:
	void autoMethods(Chrono& chrono, uint startID, uint stopID, uint durationID, uint resetID);

	void manualMethods(Chrono& chrono, uint startID, uint stopID, uint durationID, uint resetID);

	static void looseTime(uint maxIterations = 1000000);

	static void testPassed();

	static void testFailed(const string& cause);
};


#endif //CHRONO_MODETEST_H
