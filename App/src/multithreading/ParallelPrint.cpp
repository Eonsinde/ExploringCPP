#include "EntryPoint/Application.h"
#include "Classes/Reusables/Individual.h"

#include <iostream>
#include <format>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <thread>


/// @brief This example implements the Genetic Algorithm which is a search based
/// optimization technique based off Darwin's theory of evolution(Survival of the fittest)
class ParallelPrintApp : Core::Application
{
	// Iterator for vector of int
	typedef std::vector<int>::iterator VectorIntIter;

public:
	virtual void Run() override {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7,  8, 9, 10 };

		// Maximum threads to be allocated
		size_t maxThreads = (numbers.size() + MIN_PER_THREAD - 1) / MIN_PER_THREAD;

		size_t numAllocatedThreads = std::min(
			static_cast<size_t>(!NUM_AVAILABLE_CORES ? 2 : NUM_AVAILABLE_CORES),
			maxThreads
		);

		size_t blockSize = numbers.size() / numAllocatedThreads;

		std::vector<std::thread> threadsManager;
		threadsManager.reserve(numAllocatedThreads - 1);

		spdlog::info("Size of thread manager: {}", threadsManager.size());

		// Logs items in a given block
		void(*LogBlock)(VectorIntIter, VectorIntIter) = [](VectorIntIter start, VectorIntIter end) {
			for (; start != end; start++) {
				spdlog::info("{}", *start);
			}
		};

		VectorIntIter start = numbers.begin();

		for (int i = 0; i < numAllocatedThreads - 1; ++i) {
			VectorIntIter end = start;
			std::advance(end, blockSize);

			threadsManager.push_back(
				std::thread(LogBlock, start, end)
			);

			// Update start to currently processed block
			start = end;
		}

		// Let the main thread process whatever is left
		LogBlock(start, numbers.end());

		std::for_each(threadsManager.begin(), threadsManager.end(), [](std::thread& t) {
			t.join();
		});
	}

private:
	// Available processing cores
	int NUM_AVAILABLE_CORES = std::thread::hardware_concurrency();
	// Minimum elments per thread
	int MIN_PER_THREAD = 5;
};

//DECLARE_MAIN(ParallelPrintApp)

