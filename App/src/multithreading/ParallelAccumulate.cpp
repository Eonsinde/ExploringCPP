#include "EntryPoint/Application.h"

#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <mutex>
#include <numeric>


// This example shows how to use multiple threads to get the total sum of
// elements in a given data structure. It works by having each thread process
// a block/range of data in the data structure and eventually accumulate their
// results for a final output

class ParallelAccumulateExampleApp : public Core::Application
{
public:
	virtual void Run() override {
		std::cout << "Main Thread ID: " << std::this_thread::get_id() << '\n';
		std::cout << "Maximum number of threads: " << std::thread::hardware_concurrency() << "\n";
		
		//F();

		std::vector<int> numbers(200);

		// sequentially fills numbers from 1 - container end
		std::iota(numbers.begin(), numbers.end(), 1);

		int numbersSumTotal = ParallelAccumulate<std::vector<int>::iterator, int>(numbers.begin(), numbers.end(), 0);

		std::cout << "The sum total of numbers is " << numbersSumTotal << '\n';
	}

	void F() {
		// This example is useful in a situation like having to upload
		// multiple files to a server(images) and wait for the uploads to finish
		
		std::cout << "\nUsing Vector DS to manage multipe threads\n\n";

		std::vector<std::thread> threads;
		
		for (size_t i{}; i < 20; i++) {
			//std::cout << "Thread instance " << (i + 1) << " is running\n";

			threads.push_back(std::thread(
				[i, this]() {
					_mMutex.lock();

					std::cout << "Thread instance " << (i + 1) << " is running\n";
					
					_mMutex.unlock();
				}
			));
		}

		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	}

	template<typename Iterator, typename T>
	T ParallelAccumulate(Iterator iBegin, Iterator iEnd, T init) {
		// This example runs accumulation of elements in a container(vector<int>)
		// using data parallelism
		std::cout << "\nUsing Data Parallelism\n\n";

		// Check to see if the container has any items
		size_t length = std::distance(iBegin, iEnd);

		if (!length)
			return init;

		// Threshold of minimum number of elements to be handled per thread
		size_t minPerThread = 25;
		size_t maxThreads = (length + minPerThread - 1) / minPerThread;

		// Get the available number of threads on the machine
		size_t hardwareThreads = std::thread::hardware_concurrency();
		// Get the max number of threads we'll be creating
		size_t numThreads = std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads);
		// get the block size for the container which is the number of elements a given should handle
		size_t blockSize = length / numThreads;

		std::cout << "Max Number of threads: " << numThreads << '\n';
		std::cout << "Block Size: " << blockSize << '\n';

		// This stores the results for each accumulation operation by a thread
		std::vector<T> results(numThreads);
		// This holds the threads that will help process the container's data by blocks
		std::vector<std::thread> threads(numThreads - 1);
		
		// Wrapper around std::accumulate
		auto HandleAccumulate = [this](Iterator begin, Iterator end, T& result) {
			/*_mMutex.lock();
			std::cout << "Thread ID: " << std::this_thread::get_id() << '\n';
			_mMutex.unlock();*/

			result = std::accumulate(begin, end, result);
		};

		// Get the start of the first block to be processed
		Iterator blockStart = iBegin;
		
		// Doing numThreads-1 is to allow the last block to be processed on the calling thread
		// for performance gains
		for (size_t i{}; i < numThreads-1; i++) {
			Iterator blockEnd = blockStart;

			// Move the blockEnd iter to the end of the active block
			std::advance(blockEnd, blockSize);

			// Spawn a thread to process the block's data
			threads[i] = std::thread(
				HandleAccumulate,
				blockStart,
				blockEnd,
				std::ref(results[i])
			);

			// Move to next block
			blockStart = blockEnd;
		}

		// Handle the last block on the calling thread for performance
		HandleAccumulate(blockStart, iEnd, std::ref(results[numThreads - 1]));

		// Make sure to wait for all threads to complete executions
		std::for_each(threads.begin(), threads.end(), [](std::thread& instance) {
			instance.join();
		});

		// Accumulate the results container to get actual total results 
		return std::accumulate(results.begin(), results.end(), init);
	}

private:
	std::mutex _mMutex;
};

//DECLARE_MAIN(ParallelAccumulateExampleApp)
