#pragma once

#include <mutex>
#include <stdexcept>

// This class defines a custom mutex to allow to hierarchy based locking

class HierarchicalMutex {
public:
	explicit HierarchicalMutex(size_t value)
		: _mHierarchyValue{ value }, _mPreviousHierarchyValue{ 0 } {
	}

	// These are defined to suit the STL mutex standard
	void lock() {
		CheckForHierarchyViolation();
		_mInternalMutex.lock();
		UpdateHierarchyValue();
	}

	void unlock() {
		thisThreadHierarchyValue = _mPreviousHierarchyValue;
		_mInternalMutex.unlock();
	}

	bool try_lock() {
		CheckForHierarchyViolation();

		if (!_mInternalMutex.try_lock())
			return false;

		UpdateHierarchyValue();

		return true;
	}

	static size_t GetThisThreadHierarchyValue() {
		return thisThreadHierarchyValue;
	}

private:
	void CheckForHierarchyViolation() const {
		if (thisThreadHierarchyValue <= _mHierarchyValue) {
			throw std::logic_error("Mutex hierarchy violated!");
		}
	}

	void UpdateHierarchyValue() {
		_mPreviousHierarchyValue = thisThreadHierarchyValue;
		thisThreadHierarchyValue = _mHierarchyValue;
	}

	// data members
	std::mutex _mInternalMutex;
	size_t _mHierarchyValue, _mPreviousHierarchyValue;
	static inline thread_local size_t thisThreadHierarchyValue = ULONG_MAX;
};