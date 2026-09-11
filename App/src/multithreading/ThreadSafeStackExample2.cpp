#include "EntryPoint/Application.h"
#include "Classes/Threads/ThreadSafeStack.h"
#include "Utils/IPrintable.h"

#include <thread>
#include <string>
#include <vector>
#include <iostream>


// This example shows the issue with your Stack holding a type(data structure, vector<int>)
// whose copy constructors can throw exceptions due to something like memory constraints 
// when trying to perform the copy operation in an atomic function/operation
class ThreadSafeStackExample2 : public Core::Application
{
public:
	virtual void Run() override {
		// Push two elements unto stack
		_mThreadSafeStack.Push(std::vector<int>{ 12, 10, 14, 15, 16 });
		_mThreadSafeStack.Push(std::vector<int>{ 2, 5, 1, 9, 6 });
		
		// These will store the popped values from each thread
		std::vector<int> result;
		std::shared_ptr<std::vector<int>> result2Ptr;

		std::thread someThread(
			[this, &result]() {
				// FOCUS
				_mThreadSafeStack.Pop(result);

				_mMutex.lock();

				std::cout << "\nIn Thread Result one: ";
				Core::PrintContents(result.begin(), result.end());

				_mMutex.unlock();
			}
		);

		std::thread someThread2(
			[this, &result2Ptr]() {
				// FOCUS
				result2Ptr = _mThreadSafeStack.Pop();

				_mMutex.lock();

				std::cout << "\nIn Thread Result two: ";
				Core::PrintContents((*result2Ptr).begin(), (*result2Ptr).end());

				_mMutex.unlock();
			}
		);

		// This will force the EmptyStackException
		std::thread someThread3;

		/*try {
			someThread3 = std::thread(
				[this, &result]() {
					_mThreadSafeStack.pop(result);

					PrintContents(result);
				}
			);
		}
		catch(std::exception& e) {
			std::cerr << e.what() << '\n';
		}*/

		someThread.join();
		someThread2.join();
		//someThread3.join();

		std::cout << "\nResult one: ";
		Core::PrintContents(result.begin(), result.end());

		std::cout << "Result two: ";
		Core::PrintContents((*result2Ptr).begin(), (*result2Ptr).end());
	}

private:
	ThreadSafeStack<std::vector<int>> _mThreadSafeStack;
	std::mutex _mMutex;
};


//DECLARE_MAIN(ThreadSafeStackExample2)