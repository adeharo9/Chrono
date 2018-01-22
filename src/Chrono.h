#ifndef CHRONO_H
#define CHRONO_H

#include <chrono>
#include <stdexcept>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>

class Chrono
{
private:
	/* CLASS TYPEDEFS */

	typedef unsigned int uint;
	typedef long double ldouble;
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> timepoint;

public:

	/* INTERNAL ENUMS */

	enum UsageMode
	{
		MANUAL,
		AS_QUEUE,
		AS_STACK,
	};

	/* CONSTRUCTORS */

	inline Chrono() = default;

	inline explicit Chrono(UsageMode mode) : usageMode(mode)
	{
		switch (mode)
		{
			case MANUAL:
			case AS_QUEUE:
				usedIDs = Storage<>(mode);
				break;

			case AS_STACK:
				break;

			default:
				throw std::invalid_argument("Mode not yet implemented.");
		}
	}

	inline Chrono(const Chrono& chrono) = default;

	/* DESTRUCTORS */

	inline ~Chrono() = default;

	/* GET METHODS */

	inline ldouble duration (uint id) const
	{
		if (interval.at(id) < 0) throw std::logic_error("Start function must be invoked first.");

		return this -> interval.at(id);
	}

	/* MODIFIER METHODS */

	inline uint start()
	{
		uint usedID;
		try
		{
			usedID = firstAvailableID;
			++firstAvailableID;

			start(usedID);

			return usedID;
		}
		catch(std::invalid_argument& invalidArgumentException)
		{
			++firstAvailableID;

			return this -> start();
		}
	}

	inline void start (uint id)
	{
		if (usedIDs.stored(id)) throw std::invalid_argument("ID is already in use.");

		usedIDs.push(id);

		this -> interval.insert({id, -1});
		this -> stopTime.insert({id, Chrono::MAX_TIME});
		this -> startTime.insert({id, std::chrono::high_resolution_clock::now()});
	}

	inline void stop()
	{
		if (usageMode == MANUAL) throw std::logic_error("Automatic assignation/retrieval methods cannot be accessed in MANUAL execution mode.");

		uint usedID = usedIDs.first();
		stop(usedID);
	}

	inline void stop (uint id)
	{
		timepoint time = std::chrono::high_resolution_clock::now();

		if (not usedIDs.stored(id)) throw std::logic_error("Start function must be invoked first.");
		if (stopTime.at(id) != Chrono::MAX_TIME) throw std::logic_error("Stop can only be called once per stored ID.");
		if (interval.at(id) != -1) throw std::logic_error("Stop can only be called once per stored ID.");

		this -> stopTime.at(id) = time;

		this -> interval.at(id) = (std::chrono::duration<ldouble>(stopTime.at(id) - startTime.at(id))).count();
	}

	inline void reset()
	{
		if (usageMode == MANUAL) throw std::logic_error("Automatic assignation/retrieval methods cannot be accessed in MANUAL execution mode.");

		uint id = usedIDs.first();
		reset(id);
	}

	inline void reset (uint id)
	{
		if (not usedIDs.stored(id))  throw std::logic_error("This ID does not exist");

		startTime.erase(id);
		stopTime.erase(id);
		interval.erase(id);

		switch (usageMode)
		{
			case MANUAL:
				usedIDs.erase(id);
				break;

			case AS_QUEUE:
			case AS_STACK:
				usedIDs.pop();
				break;

			default:
				throw std::invalid_argument("Mode not yet implemented.");
		}
	}

private:

	template <class T = uint> class AbstractStorage
	{
		public:

			virtual AbstractStorage<T>* getCopy() const = 0;

			/* MODIFIER METHODS */

			virtual void push(T t) = 0;

			virtual T first() = 0;

			virtual void pop() = 0;

			/* CONSULTING METHODS */

			virtual bool empty() = 0;
	};

	template <class T = uint> class Storage : public AbstractStorage <T>
	{
		private:

			UsageMode usageMode = AS_STACK;

			std::unordered_set <T> storedSet;

			AbstractStorage<T>* storage = new StackStorage <T>();

		public:

			AbstractStorage<T>* getCopy() const override
			{
				return new Storage(*this);
			}

			/* CONSTRUCTION METHODS */

			inline Storage() = default;

			inline explicit Storage(UsageMode mode) : usageMode(mode)
			{
				switch (mode)
				{
					case MANUAL:
						delete storage;
						storage = nullptr;
						break;

					case AS_QUEUE:
						delete storage;
						storage = new QueueStorage <T>();
						break;

					case AS_STACK:
						break;

					default:
						throw std::invalid_argument("Mode not yet implemented.");
				}
			}

			inline Storage(const Storage& storage)
			{
				this -> usageMode = storage.usageMode;
				this -> storedSet = storage.storedSet;

				if(storage.storage != nullptr)
				{
					this -> storage = storage.storage -> getCopy();
				}
				else
				{
					this -> storage = nullptr;
				}
			}

			inline Storage& operator=(const Storage& storage)
			{
				this -> usageMode = storage.usageMode;
				this -> storedSet = storage.storedSet;

				if(storage.storage != nullptr)
				{
					this -> storage = storage.storage -> getCopy();
				}
				else
				{
					this -> storage = nullptr;
				}
			}

			/* DESTRUCTION METHODS */

			inline ~Storage()
			{
				delete storage;
			}

			/* MODIFIER METHODS */

			inline void insert(T t)
			{
				storedSet.insert(t);
			}

			inline void push(T t) override
			{
				if(storage != nullptr)
				{
					storage -> push(t);
				}

				insert(t);
			}

			inline T first() override
			{
				if(usageMode == MANUAL) throw std::logic_error("Automatic assignation/retrieval methods cannot be accessed in MANUAL execution mode.");

				return storage -> first();
			}

			inline void pop() override
			{
				if(usageMode == MANUAL) throw std::logic_error("Automatic assignation/retrieval methods cannot be accessed in MANUAL execution mode.");

				erase(this -> first());
				storage -> pop();
			}

			inline void erase(T t)
			{
				storedSet.erase(t);
			}

			/* CONSULTING METHODS */

			inline bool empty() override
			{
				return storedSet.empty();
			}

			inline bool stored(T t)
			{
				return storedSet.find(t) != storedSet.end();
			}
	};

	template <class T = uint> class QueueStorage : public  AbstractStorage <T>
	{
		private:

			std::queue <T> queueStorage;

		public:

			AbstractStorage<T>* getCopy() const override
			{
				return new QueueStorage(*this);
			}

			QueueStorage() = default;

			QueueStorage(const QueueStorage& storage)
			{
				this -> queueStorage = storage.queueStorage;
			}

			/* MODIFIER METHODS */

			inline void push(T t) override
			{
				queueStorage.push(t);
			}

			inline T first() override
			{
				return queueStorage.front();
			}

			inline void pop() override
			{
				queueStorage.pop();
			}

			/* CONSULTING METHODS */

			inline bool empty() override
			{
				return queueStorage.empty();
			}
	};

	template <class T = uint> class StackStorage : public AbstractStorage <T>
	{
		private:

			std::stack <T> stackStorage;

		public:

			AbstractStorage<T>* getCopy() const override
			{
				return new StackStorage(*this);
			}

			StackStorage() = default;

			StackStorage(const StackStorage& storage)
			{
				this -> stackStorage = storage.stackStorage;
			}

			/* MODIFIER METHODS */

			inline void push(T t) override
			{
				stackStorage.push(t);
			}

			inline T first() override
			{
				return stackStorage.top();
			}

			inline void pop() override
			{
				stackStorage.pop();
			}

			/* CONSULTING METHODS */

			inline bool empty() override
			{
				return stackStorage.empty();
			}
	};

	/* PRIVATE ATTRIBUTES */

	const timepoint MAX_TIME = timepoint::max();

	uint firstAvailableID = 0;

	UsageMode usageMode = AS_STACK;

	Storage<uint> usedIDs;

	std::unordered_map <uint, timepoint> startTime;
	std::unordered_map <uint, timepoint> stopTime;
	std::unordered_map <uint, ldouble> interval;		// In seconds
};


#endif	// CHRONO_H
