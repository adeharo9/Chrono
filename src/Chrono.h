#ifndef CHRONO_H
#define CHRONO_H

#include <chrono>
#include <stdexcept>
#include <unordered_map>

typedef unsigned int uint;
typedef long double ldouble;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> timepoint;

class Chrono
{
private:

	const timepoint MAX_TIME = timepoint::max();

	unordered_map <uint, timepoint> startTime;
	unordered_map <uint, timepoint> stopTime;
	unordered_map <uint, ldouble> interval;		// In seconds

public:

	/* CONSTRUCTORS */

	inline Chrono() = default;

	/* GET METHODS */

	inline ldouble duration (uint id) const
	{
		if (interval.at(id) < 0) throw std::logic_error("Start function must be invoked first.");

		return this -> interval.at(id);
	}

	/* MODIFIERS */

	inline void start (uint id)
	{
		if (startTime.find(id) != startTime.end()) throw std::invalid_argument("ID is already in use.");
		if (stopTime.find(id) != stopTime.end()) throw std::invalid_argument("ID is already in use.");
		if (interval.find(id) != interval.end()) throw std::invalid_argument("ID is already in use.");

		this -> interval.insert({id, -1});
		this -> stopTime.insert({id, Chrono::MAX_TIME});
		this -> startTime.insert({id, std::chrono::high_resolution_clock::now()});
	}

	inline void stop (uint id)
	{
		timepoint time = std::chrono::high_resolution_clock::now();

		if (time < startTime.at(id)) throw std::logic_error("Start function must be invoked first.");
		if (stopTime.at(id) != Chrono::MAX_TIME) throw std::logic_error("Stop can only be called once per ID.");
		if (interval.at(id) != -1) throw std::logic_error("Stop can only be called once per ID.");

		this -> stopTime.at(id) = time;

		this -> interval.at(id) = (std::chrono::duration<ldouble>(stopTime.at(id) - startTime.at(id))).count();
	}

	inline void erase (uint id)
	{
		if (startTime.find(id) == startTime.end()) throw std::logic_error("This ID does not exist");
		if (stopTime.find(id) == stopTime.end()) throw std::logic_error("This ID does not exist");
		if (interval.find(id) == interval.end()) throw std::logic_error("This ID does not exist");

		startTime.erase(id);
		stopTime.erase(id);
		interval.erase(id);
	}
};


#endif	// CHRONO_H
