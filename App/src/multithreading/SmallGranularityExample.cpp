#include "EntryPoint/Application.h"

#include <spdlog/spdlog.h>
#include <thread>
#include <mutex>


// This example shows where using a fine-grained lock makes perfect
// sense and is more optimal. The logic here is simple, the comparison
// operator of Y only protects Y's data when it needs be accessed; the
// actual comparison operation is then done on copies since Y's data is
// an integer and quite memory cheap to copy

class Y
{
public:
	Y(const int value) : _mData{ value }{}

	void SetData(const int value) {
		std::lock_guard<std::mutex> lg(_mMutex);
		_mData = value;
	}

	// We protect the data before we read it
	int GetData() const {
		std::lock_guard<std::mutex> lg(_mMutex);
		return _mData;
	}

	// We compare the data here using copies since locking both instances of Y
	// would rather be redundant since int copying is memory cheap
	bool operator==(const Y& rhs) {
		if (this == &rhs)
			return true;

		// Using GetData is crucial here to ensure the data of each instance is
		// protected incase another thread were accessing it simultaneously
		const int lhsData = GetData();
		const int rhsData = rhs.GetData();

		// This is the best place to log what is being compared as the operands
		// will certainly safe/unchanged
		spdlog::info("{} is being compared to {}", lhsData, rhsData);

		return lhsData == rhsData;
	}

private:
	mutable std::mutex _mMutex;
	int _mData;
};


class SmallGranularityExample : public Core::Application
{
public:
	virtual void Run() override {
		Y instance1{ 12 };
		Y instance2{ 15 };
		Y instance3{ 21 };

		// NB: I intentionally allow a thread91 to change the value of instance3 to
		// make things more entertaining and buggy

		std::thread t32{
			[&instance1, &instance3] {
				// If this operations successfully acquires the lock for instance3 before
				// t91, then the comparison would be 12 == 21
				if (instance1 == instance3) {
					// The values being logged here will likely not be what is compared as t91
					// will probably have update instance3's data value to 90
					spdlog::info("{} equals to {}", instance1.GetData(), instance3.GetData());
				}
				else {
					spdlog::info("{} not equals to {}", instance1.GetData(), instance3.GetData());
				}
			}
		};

		std::thread t91{
			[&instance3] {
				instance3.SetData(90);
			}
		};

		t32.join();
		t91.join();
	}
};

//DECLARE_MAIN(SmallGranularityExample)
