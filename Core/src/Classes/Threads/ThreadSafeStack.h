#pragma once

#include <exception>
#include <mutex>
#include <stack>
#include <memory>

class EmptyStackException : public std::exception
{
	virtual const char* what() const noexcept override {
		return "An Exception Occurred: Empty Stack!";
	}
};

template<typename T>
class ThreadSafeStack
{
public:
	ThreadSafeStack() = default;
	ThreadSafeStack(const ThreadSafeStack& rhs) {
		// Ensure no other thread is accessing the stack to be copied
		std::lock_guard<std::mutex> LG(rhs._mMutex);

		_mData = rhs._mData;
	}
	// delete copy
	ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

	void Push(T value) {
		std::lock_guard<std::mutex> LG(_mMutex);

		_mData.push(value);
	}

	// Returns a pointer to an element popped off the stack
	std::shared_ptr<T> Pop() {
		std::lock_guard<std::mutex> LG(_mMutex);

		// Ensure stack isn't empty
		if (_mData.empty()) throw EmptyStackException();
		// Copy entry to be popped into new memory
		std::shared_ptr<T> ptr = std::make_shared<T>(_mData.top());
		// Remove entry from stack
		_mData.pop();
		
		return ptr;
	}

	// Remove an element from stack but stores the item in out
	void Pop(T& out) {
		std::lock_guard<std::mutex> LG(_mMutex);

		// Ensure stack isn't empty
		if (_mData.empty()) throw EmptyStackException();
		// Store entry to be popped in output variable - out
		out = _mData.top();
		// Remove entry from stack
		_mData.pop();
	}

	bool Empty() const {
		std::lock_guard<std::mutex> LG(_mMutex);

		return _mData.empty();
	}

private:
	std::stack<T> _mData;
	std::mutex _mMutex;
};