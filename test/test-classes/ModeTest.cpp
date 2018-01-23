#include "ModeTest.h"
#include <iostream>

using namespace std;

void ModeTest::initialize()
{
	usageModes = vector<Chrono::UsageMode>({Chrono::MANUAL, Chrono::AS_QUEUE, Chrono::AS_STACK});
}

void ModeTest::run()
{
	Chrono chrono;
	for(Chrono::UsageMode usageMode : usageModes)
	{
		this -> usageMode = usageMode;

		chrono = Chrono(usageMode);

		try
		{
			manualMethods(chrono, 0, 0, 0, 0);

			if(testState == END_EXECUTION)
			{
				cout << TestStateTranslation::getStrDescriptor(testState) + " [no exception]: Test didn't throw an exception, as it shouldn't have to." << endl;
				testPassed();
			}
			else
			{
				throw invalid_argument(TestStateTranslation::getStrDescriptor(testState) + " [no exception]: Bad test termination state (should have been END_EXECUTION)");
			}
		}
		catch (exception& thrownException)
		{
			testFailed(TestStateTranslation::getStrDescriptor(testState) + " [exception]: " + thrownException.what());
		}

		chrono = Chrono(usageMode);

		try
		{
			manualMethods(chrono, 0, 0, 0, 1);

			testFailed(TestStateTranslation::getStrDescriptor(testState) + " [no exception]: Test should have thrown an exception at state BEGIN_RESET");
		}
		catch (exception& thrownException)
		{
			if(testState == BEGIN_RESET)
			{
				cout << TestStateTranslation::getStrDescriptor(testState) + " [exception]: Test threw an exception, as it should have to." << endl;
				testPassed();
			}
			else
			{
				cout << TestStateTranslation::getStrDescriptor(testState) << " [exception]: Bad test termination state (should have been BEGIN_RESET)" << endl;
				testFailed(thrownException.what());
			}
		}

		chrono = Chrono(usageMode);

		try
		{
			manualMethods(chrono, 0, 0, 1, 0);

			testFailed(TestStateTranslation::getStrDescriptor(testState) + " [no exception]: Test should have thrown an exception at state BEGIN_DURATION");
		}
		catch (exception& thrownException)
		{
			if(testState == BEGIN_DURATION)
			{
				cout << endl;
				cout << TestStateTranslation::getStrDescriptor(testState) + " [exception]: Test threw an exception, as it should have to." << endl;
				testPassed();
			}
			else
			{
				cout << TestStateTranslation::getStrDescriptor(testState) << " [exception]: Bad test termination state (should have been BEGIN_DURATION)" << endl;
				testFailed(thrownException.what());
			}
		}

		chrono = Chrono(usageMode);

		try
		{
			manualMethods(chrono, 0, 1, 0, 0);

			testFailed(TestStateTranslation::getStrDescriptor(testState) + " [no exception]: Test should have thrown an exception at state BEGIN_STOP");
		}
		catch (exception& thrownException)
		{
			if(testState == BEGIN_STOP)
			{
				cout << TestStateTranslation::getStrDescriptor(testState) + " [exception]: Test threw an exception, as it should have to." << endl;
				testPassed();
			}
			else
			{
				cout << TestStateTranslation::getStrDescriptor(testState) << " [exception]: Bad test termination state (should have been BEGIN_STOP)" << endl;
				testFailed(thrownException.what());
			}
		}
	}
}

void ModeTest::dataProcessing()
{

}

void ModeTest::end()
{

}

void ModeTest::autoMethods(Chrono& chrono, uint startID, uint stopID, uint durationID, uint resetID)
{
	chrono.start();
	looseTime();
	chrono.stop();

}

void ModeTest::manualMethods(Chrono& chrono, uint startID, uint stopID, uint durationID, uint resetID)
{
	testState = BEGIN_EXECUTION;
	cout << "Manual methods test:" << endl;
	cout << "	- Usage mode: " << Chrono::UsageModeTranslation::getStrDescriptor(usageMode) << endl;
	cout << "	- start("<< startID << ")" << endl;
	cout << "	- stop(" << stopID << ")" << endl;
	cout << "	- duration(" << durationID << ")" << endl;
	cout << "	- reset(" << resetID << ")" << endl;
	cout << "		> Starting chrono." << endl;

	testState = BEGIN_START;
	chrono.start(startID);
	testState = END_START;

	cout << "		> Chrono started." << endl;

	testState = BEGIN_LOOSE_TIME;
	looseTime();
	testState = END_LOOSE_TIME;

	cout << "		> Time lost." << endl;

	testState = BEGIN_STOP;
	chrono.stop(stopID);
	testState = END_STOP;

	cout << "		> Chrono stopped." << endl;
	cout << "		> Chrono duration: ";

	testState = BEGIN_DURATION;
	cout << chrono.duration(durationID) << endl;
	testState = END_DURATION;

	cout << "		> Resetting chrono." << endl;

	testState = BEGIN_RESET;
	chrono.reset(resetID);
	testState = END_RESET;

	cout << "		> Chrono reset." << endl;
	testState = END_EXECUTION;
}

void ModeTest::looseTime(uint maxIterations)
{
	for(uint i = 0; i < maxIterations; ++i);
}

void ModeTest::testPassed()
{
	cout << "TEST PASSED." << endl << endl;
}

void ModeTest::testFailed(const string& cause)
{
	cout << "TEST FAILED." << endl;
	cout << "Cause:" << endl;
	throw runtime_error(cause);
}