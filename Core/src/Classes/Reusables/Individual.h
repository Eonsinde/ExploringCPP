#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include "Classes/Randoms/RandomNumberGenerator.h"
#include <string>
#include <format>

// A reusable class that mimics an Human with chromosome level details;
// most suitable for evolutionary algorithms
class Individual
{
public:
    Individual(const std::string& xy, const std::string& target);

    // Create genome based on the target
    static std::string CreateGenome(const std::string& target);

    // Creates a new Individual by mating
    Individual Mate(const Individual& rhs);

    // Getters

    std::string GetDetails() const;
    std::string GetChromosome() const;
    int GetFitness() const;
    const std::string& GetPossibleGenes();

protected:
    // Generated a random gene from the genes
    static char MutatedGenes(const std::string& genes);

    // Get hamming distance: how much of instance's chromosomes don't match with target
    int CalcFitness(const std::string& target) {
        int fitness = 0;

        for (int i{}; i < target.size(); ++i) {
            if (_mChromosome[i] != target[i])
                fitness += 1;
        }
        
        // If zero, the instance matches with the given target
        return fitness;
    }

private:
    // Holds all possible genes an individual can have. It can not be modified hence it been private
    static inline std::string _mPossibleGenes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 1234567890, .-;:_!\"#%&/()=?@${[]}";
    // Very dangerous as reference string could be freed from memory
    const std::string* _mTarget;
    std::string _mChromosome;
    int _mFitness;
};

#endif 