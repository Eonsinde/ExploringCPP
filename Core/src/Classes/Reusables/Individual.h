#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include "Classes/Randoms/RandomNumberGenerator.h"
#include <string>
#include <format>


namespace Core{
    /// @brief A reusable class that mimics an Human with chromosome level details;
    /// most suitable for evolutionary algorithms
    class Individual
    {
    public:
        /// @brief Default constructor
        /// @param xy Chromosomes for new individual
        /// @param target The targeted chromosome - used to calculate individual's fitness
        Individual(const std::string& xy, const std::string& target);

        /// @brief Creates the a genome based off a given target
        /// @param target Item to reference when creating genome
        /// @return New chromosome for an individual
        static std::string CreateGenome(const std::string& target);

        /// @brief Creates a new Individual by mating
        /// @param rhs The second individual to be mated with
        /// @return New Individual
        Individual Mate(const Individual& rhs);

        // ----------------------------- Getters -----------------------------

        /// @brief Retrieves details about an Individual
        /// @return Chromosome + Fitness 
        std::string GetDetails() const;

        /// @brief Retrieves chromosome for Individual
        /// @return Chromosome
        std::string GetChromosome() const;

        /// @brief Retrieves fitness for Individual
        /// @return Fitness
        int GetFitness() const;

        /// @brief Retrieves the possible genes an Individual can be derived from
        /// @return possible genes
        static const std::string& GetPossibleGenes();

    protected:
        // Generated a random gene from the genes
        static char MutatedGenes(const std::string& genes);

        // Get hamming distance: how much of instance's chromosomes don't match with target
        int CalcFitness(const std::string& target);

    private:
        // Holds all possible genes an individual can have. It can not be modified hence it been private
        static inline std::string _mPossibleGenes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 1234567890, .-;:_!\"#%&/()=?@${[]}";
        // Very dangerous as reference string could be freed from memory
        const std::string* _mTarget;
        std::string _mChromosome;
        int _mFitness;
    };
}

#endif 