#include "EntryPoint/Application.h"

#include <spdlog/spdlog.h>
#include <thread>
#include <mutex>

// This example shows how to transfer the ownership of a mutex
// between instances of a lock(std::unique_lock). It shows an
// hypothetical use-case where such could possibly fit
class LockTransferExample : public Core::Application
{
public:
	virtual void Run() override {
		std::thread t90(
			[this] {
				ProcessData();
			}
		);

		std::thread t35(
			[this] {
				ProcessData();
			}
		);

		t90.join();
		t35.join();
	}

	// Do some processing
	void PrepareData() {
		spdlog::info("Preparing data...");
	}

	// Trigger the prepare data operation and return the lock
	std::unique_lock<std::mutex> GetLock() {
		std::unique_lock<std::mutex> ul(_mMutex);

		PrepareData();

		return ul;
	}

	// Process the prepared data
	void ProcessData() {
		std::unique_lock<std::mutex> returnedLock{ GetLock() };

		// Prepared data is being processed safely since the lock is still active
		spdlog::info("Processing data...");
	}

private:
	std::mutex _mMutex;
};

//DECLARE_MAIN(LockTransferExample)