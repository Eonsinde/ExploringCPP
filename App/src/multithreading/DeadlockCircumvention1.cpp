#include "EntryPoint/Application.h"
#include "Classes/Threads/HierarchicalMutex.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <string>


// This example shows how to use std::lock to prevent deadlock when 
// multiple threads are trying to modify two instances of a class.
// In this case, we go with a simple swap operation of data between the given instances.
// The point of this example is to make sure another thread cannot perform the swap
// operation while an active thread is doing that

class TestObject {
	// ADL(Argument Dependent Lookup)
	// Note, the lowercase is important to match with the STL generic swap in case of a fallback.
	friend void swap(TestObject& lhs, TestObject& rhs) {
		// Call member function
		lhs.Swap(rhs);
	}

public:
	TestObject(): _mData{} {}
	TestObject(std::string s): _mData{s} {}

	void Print(const std::string& name) const {
		std::lock_guard<std::mutex> LG(_mMutex);
		std::cout << name << " contains: " << _mData << "\n";
	}

	void Swap(TestObject& rhs) {
		// Ensure the objects are different
		if (this == &rhs)
			return;

		std::lock(_mMutex, rhs._mMutex);
		std::lock_guard<std::mutex> LG1(_mMutex, std::adopt_lock);
		std::lock_guard<std::mutex> LG2(rhs._mMutex, std::adopt_lock);
		
		// Perform actual swap here
		std::swap(_mData, rhs._mData);
	}

private:
	std::string _mData;
	mutable std::mutex _mMutex;
};

class DeadlockCircumvention1 : public Core::Application
{
public:
	virtual void Run() override {
		
	}

	// FOCUS: Argument Dependent Lookup
	void Test1() {
		TestObject obj1("2 Files");
		TestObject obj2("8 Files");

		// Before swap
		obj1.Print("Object_1");
		obj2.Print("Object_2");

		// Using your member function Swap
		obj1.Swap(obj2);
		std::cout << '\n';

		// After swap
		obj1.Print("Object_1");
		obj2.Print("Object_2");

		// Generic programming using ADL
		using std::swap;
		swap(obj1, obj2);

		std::cout << '\n';

		// After ADL
		obj1.Print("Object_1");
		obj2.Print("Object_2");
	}

	// FOCUS: Perform concurrent swap on two instances of the TestObject class
	void Test2() {
		TestObject obj1("2 Files");
		TestObject obj2("8 Files");

		// Before Swap
		std::cout << "========== Before Swap ==========\n";
		obj1.Print("Object_1");
		obj2.Print("Object_2");

		// Concurrent modifications
		std::thread t1(
			[&obj1, &obj2]() {
				for (int i{}; i < 1000; i++) {
					using std::swap;
					swap(obj1, obj2);
				}
			}
		);

		std::thread t2(
			[&obj1, &obj2]() {
				for (int i{}; i < 1000; i++) {
					using std::swap;
					swap(obj2, obj1);
				}
			}
		);

		// Wait for threads to finish
		t1.join();
		t2.join();

		// After Swap
		std::cout << "========== After Swap ==========\n";
		obj1.Print("Object_1");
		obj2.Print("Object_2");
	}
};

//DECLARE_MAIN(DeadlockCircumvention1)