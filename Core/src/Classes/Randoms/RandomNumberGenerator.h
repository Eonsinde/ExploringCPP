#ifndef __RANDOM_NUMBER_GENERATOR__H__
#define __RANDOM_NUMBER_GENERATOR__H__

#include <random>
#include <memory>

// This class helps generate random numbers within a given range
// and operates as a singleton
class RandomNumberGenerator
{
public:
    // No copy allowed
    RandomNumberGenerator(const RandomNumberGenerator&) = delete;
    RandomNumberGenerator& operator=(const RandomNumberGenerator&) = delete;

    // Returns integer within the given range - parameters inclusive
    int Generate(int min, int max) {
        std::uniform_int_distribution<int> distrib(min, max);

        return distrib(_mGen);
    }

    static RandomNumberGenerator& GetInstance() {
        if (!_mInstance)
            // Constructor new instance here on heap
            _mInstance = std::make_unique<RandomNumberGenerator>();

        return *_mInstance;
    }

private:
    static inline std::unique_ptr<RandomNumberGenerator> _mInstance = nullptr;
    std::mt19937 _mGen;

    // Private constructor: the outside world knows nothing about this
    RandomNumberGenerator() {
        std::random_device rd;
        _mGen.seed(rd());
    }
};

#endif

