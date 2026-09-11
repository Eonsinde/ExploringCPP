#include "EntryPoint/Application.h"
#include "Classes/Threads/ThreadSafeStack.h"

#include <iostream>
#include <stack>
#include <mutex>
#include <thread>
#include <string>
#include <vector>
#include <optional>


// This example showcases how to use internal locking(in classnamspace) to combine multiple
// operations on a shared resource between multiple threads and safely 
// circumvent context switching leading to broken invariants on the resource
class ThreadSafeStackExampleApp : public Core::Application
{
public:
	virtual void Run() override {
		_mThreadSafeStack.Push("Element A");
		_mThreadSafeStack.Push("Element B");

		// TODO: Introduce mutexes to protect the cout object 
		// Context switching still occurs when using `cout` between both threads here
		// since there's no synchronization to protect the `cout` object

		std::thread thread1(
			[this]() {
				// FOCUS
				auto item = _mThreadSafeStack.Pop();

				if (item) {
					std::cout << "Thread 1 popped: " << *item << '\n';
				}
				else {
					std::cout << "Thread 1 found an empty stack\n";
				}
			}
		);

		std::thread thread2(
			[this]() {
				// FOCUS
				auto item = _mThreadSafeStack.Pop();

				if (item) {
					std::cout << "Thread 2 popped: " << *item << '\n';
				}
				else {
					std::cout << "Thread 2 found an empty stack\n";
				}
			}
		);

		thread1.join();
		thread2.join();
	}

private:
	ThreadSafeStack<std::string> _mThreadSafeStack;
};

//DECLARE_MAIN(ThreadSafeStackExampleApp)