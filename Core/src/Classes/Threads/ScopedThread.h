#pragma once

#include <thread>
#include <stdexcept>

// ScopedThread: Takes owner of a given thread instance
// It is an upgrade from ThreadGuard and helps avoid any unpleasant consequences
// should the thread_guard object outlive the thread it was referencing
class ScopedThread
{
public:
	ScopedThread(std::thread t)
		: _mThread{ std::move(t) } {
		// Ensure thread isn't joinable(joined/detached)
		if (!_mThread.joinable())
			throw std::logic_error("ScopedThread Constructor::Thread instance argument must be joinabled");
	}

	ScopedThread(ScopedThread&& rhs) noexcept {
		// join the currently referenced thread to ensure completion
		_mThread.join();
		// update its data to the new value from rhs instance
		_mThread = std::move(rhs._mThread);
	}
	
	ScopedThread(const ScopedThread&) = delete;
	
	~ScopedThread() {
		// Make sure to join to calling thread to prevent exception
		_mThread.join();
	}

	ScopedThread& operator=(const ScopedThread&) = delete;

	ScopedThread& operator=(const ScopedThread&& rhs) noexcept {
		return *this;
	}

private:
	std::thread _mThread;
};
