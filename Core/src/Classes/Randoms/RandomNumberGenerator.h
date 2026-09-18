#ifndef __RANDOM_NUMBER_GENERATOR__H__
#define __RANDOM_NUMBER_GENERATOR__H__

#include <random>
#include <memory>

namespace Core {
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
            if (min >= max)
                throw std::logic_error("RandomNumberGenerator: Min must be lesser than Max!");

            std::uniform_int_distribution<int> distrib(min, max);

            return distrib(_mGen);
        }

        static RandomNumberGenerator& GetInstance() {
            // Instance is created once here
            static RandomNumberGenerator _mInstance;        
            return _mInstance;
        }

    private:
        std::mt19937 _mGen;

        // Private constructor: the outside world knows nothing about this
        RandomNumberGenerator() {
            std::random_device rd;
            _mGen.seed(rd());
        }
    };
}

#endif

