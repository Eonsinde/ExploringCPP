#include "Individual.h"

Individual::Individual(const std::string& xy, const std::string& target)
    : _mChromosome{ xy }, _mTarget{ &target }, _mFitness{ CalcFitness(*_mTarget) } {
        
}

std::string Individual::CreateGenome(const std::string& target) {
    std::string genome;
    genome.reserve(target.size());

    for (int i{}; i < target.size(); ++i) {
        genome.push_back(MutatedGenes(_mPossibleGenes));
    }

    return genome;
}

Individual Individual::Mate(const Individual& rhs) {
    std::string newChromosome;
    newChromosome.reserve((*_mTarget).size());

    for (int i{}; i < (*_mTarget).size(); ++i) {
        int prob = RandomNumberGenerator::GetInstance().Generate(1, 10);

        if (prob < 4) {
            // Crossover: select gene from parent 1
            newChromosome.push_back(_mChromosome[i]);
        } else if (prob < 8) {
            // Crossover: select gene from parent 2
            newChromosome.push_back(rhs._mChromosome[i]);
        } else {
            // Mutation: randomly select some gene
            newChromosome.push_back(MutatedGenes(_mPossibleGenes));
        }
    }

    // Create new individual based on chromosome
    return Individual(newChromosome, *_mTarget);
}

std::string Individual::GetDetails() const {
    return std::format("Chromosome: {} | Fitness: {}", _mChromosome, _mFitness);
}

std::string Individual::GetChromosome() const {
    return _mChromosome;
}

int Individual::GetFitness() const {
    return _mFitness;
}

const std::string& Individual::GetPossibleGenes() {
    return _mPossibleGenes;
}

// Generated a random gene from the genes
char Individual::MutatedGenes(const std::string& genes) {
    int randomIndex = RandomNumberGenerator::GetInstance().Generate(0, genes.size()-1);
    return genes[randomIndex];
}

int Individual::CalcFitness(const std::string& target) {
    int fitness = 0;

    for (int i{}; i < target.size(); ++i) {
        if (_mChromosome[i] != target[i])
            fitness += 1;
    }
    
    // If zero, the instance matches with the given target
    return fitness;
}


