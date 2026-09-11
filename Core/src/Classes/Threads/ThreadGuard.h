#ifndef __THREADGUARD_H__
#define __THREADGUARD_H__

#include <thread>

// ThreadGuard: This class ensures the referenced thread instance is joined before
// its destructor gets called, preventing the std::terminate exception
class ThreadGuard {
private:
	std::thread& _mThreadInstance;

public:
	ThreadGuard(std::thread& t): _mThreadInstance(t) { }
	~ThreadGuard() {
		if (_mThreadInstance.joinable()) {
			// awaits thread completion on the calling thread if necessary
			_mThreadInstance.join();
		}
	}

	// prevent copy constr
	ThreadGuard(const ThreadGuard&) = delete;
	// prevent copy op
	ThreadGuard& operator=(const ThreadGuard&) = delete;
};

#endif