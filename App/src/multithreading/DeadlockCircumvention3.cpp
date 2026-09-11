#include "EntryPoint/Application.h"

#include <spdlog/spdlog.h>
#include <thread>
#include <mutex>


// This example show how to use std::unique_lock for more flexibility
// with sychronization to circumvent potential deadlocks

class SomeObject {
	// ADL(Argument Dependent Lookup)
	// Note, the lowercase is important to match with the STL generic swap in case of a fallback.
	friend void swap(SomeObject& lhs, SomeObject& rhs) {
		// Call member function
		lhs.Swap(rhs);
		
		// Using sequential lock acquisition for swap: prone to deadlock during usage though
		//lhs.SequentialLockOnSwap(rhs);
	}

public:
	SomeObject() : _mData{} {}
	SomeObject(std::string s) : _mData{ s } {}

	void Print(const std::string& name) const {
		std::lock_guard<std::mutex> LG(_mMutex);
		spdlog::info("{} contains {}", name, _mData);
	}

	void Swap(SomeObject& rhs) {
		// Ensure the objects are different
		if (this == &rhs)
			return;

		// FOCUS: using unique_lock
		std::unique_lock<std::mutex> UL1(_mMutex, std::defer_lock);
		std::unique_lock<std::mutex> UL2(rhs._mMutex, std::defer_lock);
		std::lock(UL1, UL2);

		// Perform actual swap here
		std::swap(_mData, rhs._mData);
	}

	// NB: This function doesnt't use std::lock and can be substituted with Swap but the
	// order of acquiring locks between threads must be consistent, else a deadlock will occur
	void SequentialLockOnSwap(SomeObject& rhs) {
		// Ensure the objects are different
		if (this == &rhs)
			return;

		// FOCUS: acquiring locks sequentially using unique_lock
		std::unique_lock<std::mutex> UL1(_mMutex);
		std::unique_lock<std::mutex> UL2(rhs._mMutex);

		// Perform actual swap here
		std::swap(_mData, rhs._mData);
	}

private:
	std::string _mData;
	mutable std::mutex _mMutex;
};

class DeadlockCircumvention3 : public Core::Application
{
public:
	virtual void Run() override {
		SomeObject obj1("Hello");
		SomeObject obj2("World");

		std::cout << "========== Before Swap ==========\n";
		obj1.Print("Object 1");
		obj2.Print("Object 2");

		// Perform swap operation multiple times on secondary and main threads
		std::thread t9(
			[&]() {
				using std::swap;
				
				for (int i{}; i < 5; i++) {
					swap(obj1, obj2);
				}
			}
		);

		std::thread t19(
			[&]() {
				using std::swap;

				for (int i{}; i < 5; i++) {
					// NB: this is perfectly fine when using the std::lock in swap operation
					swap(obj2, obj1);
				}
			}
		);

		for (int i{}; i < 5; i++) {
			swap(obj1, obj2);
		}

		t9.join();

		std::cout << "\n\n========== After Swap ==========\n";
		obj1.Print("Object 1");
		obj2.Print("Object 2");
	}
};

//DECLARE_MAIN(DeadlockCircumvention3)