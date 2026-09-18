// This module contains helper functions for custom thread operation
#include <utility>
#include <thread>

namespace Core {
    /// @brief Helps derive the number of blocks and threads needed to process some data
    /// @param length The size of the data to be processed
    /// @param minPerThread Minimum amount of elements per block
    /// @return The number of threads, block size
    std::pair<size_t, size_t> ProcessByBlocks(size_t length, size_t minPerThread=50)
    {
        static const size_t NUM_AVAILABLE_CORES = std::thread::hardware_concurrency();

        // Get the maximum number of threads needed for the full data
        size_t maxThreads = (length + minPerThread - 1) / minPerThread;

        size_t numAllocatedThreads = std::min(
            !NUM_AVAILABLE_CORES ? 2 : NUM_AVAILABLE_CORES,
            maxThreads
        );

        // Get the block size a thread can process
        size_t blockSize = length / numAllocatedThreads;

        return std::pair<size_t, size_t>(numAllocatedThreads, blockSize);
    }
}