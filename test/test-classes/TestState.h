#ifndef CHRONO_TESTSTATE_H
#define CHRONO_TESTSTATE_H

#include <stdexcept>
#include <string>

enum TestState
{
	INITIALIZE_STATE,
	BEGIN_EXECUTION,
	BEGIN_START,
	END_START,
	BEGIN_LOOSE_TIME,
	END_LOOSE_TIME,
	BEGIN_STOP,
	END_STOP,
	BEGIN_DURATION,
	END_DURATION,
	BEGIN_RESET,
	END_RESET,
	END_EXECUTION,
	DELETE_STATE,
};

class TestStateTranslation
{
public:
	static std::string getStrDescriptor(TestState testState)
	{
		std::string strTestState;

		switch (testState)
		{
			case INITIALIZE_STATE:
				strTestState = "INITIALIZE_STATE";
				break;
			case BEGIN_EXECUTION:
				strTestState = "BEGIN_EXECUTION";
				break;
			case BEGIN_START:
				strTestState = "BEGIN_START";
				break;
			case END_START:
				strTestState = "END_START";
				break;
			case BEGIN_LOOSE_TIME:
				strTestState = "BEGIN_LOOSE_TIME";
				break;
			case END_LOOSE_TIME:
				strTestState = "END_LOOSE_TIME";
				break;
			case BEGIN_STOP:
				strTestState = "BEGIN_STOP";
				break;
			case END_STOP:
				strTestState = "END_STOP";
				break;
			case BEGIN_DURATION:
				strTestState = "BEGIN_DURATION";
				break;
			case END_DURATION:
				strTestState = "END_DURATION";
				break;
			case BEGIN_RESET:
				strTestState = "BEGIN_RESET";
				break;
			case END_RESET:
				strTestState = "END_RESET";
				break;
			case END_EXECUTION:
				strTestState = "END_EXECUTION";
				break;
			case DELETE_STATE:
				strTestState = "DELETE_STATE";
				break;
			default:
				throw std::invalid_argument("Test State not yet implemented.");
		}

		return strTestState;
	}
};

#endif //CHRONO_TESTSTATE_H
