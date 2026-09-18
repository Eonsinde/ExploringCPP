#include "EntryPoint/Application.h"
#include "Classes/Reusables/Individual.h"
#include "Classes/Threads/Helpers.h"

#include <iostream>
#include <format>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <functional>


/// @brief This example implements the Genetic Algorithm which is a search based
/// optimization technique based off Darwin's theory of evolution(Survival of the fittest)
class GAApplication : Core::Application
{
	// Iterator for vector of int
	using VectorIntIter = std::vector<int>::iterator;
	// Iterator for vector of Individual
	using VectorIndividualIter = std::vector<Core::Individual>::iterator;

public:
	virtual void Run() override {
		int populationSizes[]{ 10, 20, 50, 100 };

		//GeneticAlgorithm(populationSizes[0], 10000, true);
		ThreadSafeGeneticAlgorithm(populationSizes[2], 5000, true, true);
	}

	std::shared_ptr<std::vector<int>> GeneticAlgorithm(
		int popSize, int maxGenerations=3000,
		bool verbose=true, bool extraVerbose=false
	) {
		// Starting generation
		int generation = 1;
		// Fitness history for a given population
		std::shared_ptr<std::vector<int>> fitnessHistory = std::make_shared<std::vector<int>>();
		
		// Holds the population for a given generation
		std::vector<Core::Individual> population;
		population.reserve(popSize);
		// Generate populace
		for (int i = 0; i < popSize; ++i) {
			population.push_back(Core::Individual{ Core::Individual::CreateGenome(TARGET), TARGET });
		}

		if (verbose) {
			spdlog::info("========== Starting Population ==========");

			for (int i = 0; i < popSize; ++i) {
				spdlog::info(population.at(i).GetDetails());
			}

			spdlog::info("=========================================");
		}
		

		do {
			// Sort by fitness so the best Individual is on top
			std::sort(population.begin(), population.end(), [](Core::Individual& lhs, Core::Individual& rhs){
				return lhs.GetFitness() < rhs.GetFitness();
			});

			int bestFitness = population.at(0).GetFitness();
			fitnessHistory->push_back(bestFitness);

			// Log info about the fittest individual for the given generation
			if (verbose)
				spdlog::info(
					"Gen: {}\tSolution: {}\tFitness Score: {}",
					generation, population.at(0).GetChromosome(), bestFitness
				);

			// base case for loop
			if (bestFitness == 0 || generation >= maxGenerations)
				break;

			// The algorithm takes off from here fully
			std::vector<Core::Individual> newGeneration;
			newGeneration.reserve(popSize);

			// Elitism: preserve a certain percentage of the fittest individuals
			// in each generation -  in our case, 10% from the previous generation
			int elitismPoolSize = 0.1f * popSize;

			for (int i = 0; i < elitismPoolSize; i++) {
				newGeneration.push_back(population.at(i));
			}

			// The rest of the population is created by mating. We preferentially
			// choose parents from the better half(50%) of the population in this process
			int matePoolSize = 0.5f * popSize;
			
			// TODO(GeneticAlgorithm): optimize this by possibly refactoring the Individual
			// class to support a no param constructor.
			// Since we already have 10% of the newGeneration, get the remaining 90%
			for (int i = 0; i < (popSize - elitismPoolSize); i++) {
				// Get a random index within the range of 50% of the populace and use to retrieve a parent
				Core::Individual parent1 = population.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));
				Core::Individual parent2 = population.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));
				
				newGeneration.push_back(parent1.Mate(parent2));
			}

			// Update current population with new generation
			population = newGeneration;
			generation += 1;
		} while (true);

		return fitnessHistory;
	}

	std::shared_ptr<std::vector<int>> ThreadSafeGeneticAlgorithm(
		int popSize, int maxGenerations = 3000,
		bool verbose = false, bool extraVerbose = false
	) {
		// Starting generation
		int generation = 1;
		// Fitness history for a given population
		std::shared_ptr<std::vector<int>> fitnessHistory = std::make_shared<std::vector<int>>();

		// Holds the population for a given generation
		std::vector<Core::Individual> population;
		population.reserve(popSize);
		// Generate populace
		for (int i = 0; i < popSize; ++i) {
			population.push_back(Core::Individual{ Core::Individual::CreateGenome(TARGET), TARGET });
		}

		// GA kicks off here
		do {
			// Sort by fitness so the best Individual is on top
			std::sort(population.begin(), population.end(), [](Core::Individual& lhs, Core::Individual& rhs) {
				return lhs.GetFitness() < rhs.GetFitness();
			});

			// Start details log here
			if (verbose)
				spdlog::info("========== Generation {} ==========", generation);


			// Log the current population per generation
			if (extraVerbose) {
				try {
					for (int i = 0; i < popSize; ++i) {
						spdlog::info(population.at(i).GetDetails());
					}
				}
				catch (std::exception& e) {
					spdlog::error("{}", e.what());
				}
			}

			// Get fitness individual
			int bestFitness = population.at(0).GetFitness();
			fitnessHistory->push_back(bestFitness);

			// Log the fittest individual for the given generation
			if (verbose && !extraVerbose)
				spdlog::info(
					"Gen: {}\tSolution: {}\tFitness Score: {}\n",
					generation, population.at(0).GetChromosome(), bestFitness
				);

			// base case for loop
			if (bestFitness == 0 || generation >= maxGenerations)
				break;

			// The algorithm takes off from here fully
			std::vector<Core::Individual> newPopulation;
			newPopulation.reserve(popSize);

			// Elitism: preserve a certain percentage of the fittest individuals
			// in each generation -  in our case, 10% from the previous generation
			size_t elitismPoolSize = 0.1f * popSize;

			for (int i = 0; i < elitismPoolSize; i++) {
				newPopulation.push_back(population.at(i));
			}

			// The rest of the population is created by mating. We preferentially
			// choose parents from the better half(50%) of the population in this process
			size_t matePoolSize = 0.5f * popSize;
			// How much of the populace is yet to be filled
			size_t remainingPoolSize = popSize - elitismPoolSize;

			// Get the needed thread number and block size
			std::pair<size_t, size_t> processMetadata = Core::ProcessByBlocks(remainingPoolSize);

			std::vector<std::thread> threadsManager;
			threadsManager.reserve(processMetadata.first - 1);

			// Start filling up past the perserved elitism populace
			size_t startIndex = elitismPoolSize;

			// Spawn the needed threads
			for (int i = 0; i < processMetadata.first - 1; ++i) {
				size_t endIndex = elitismPoolSize + processMetadata.second;

				threadsManager.emplace_back(
					[=, &population, &newPopulation, &matePoolSize] () mutable {
						for (; startIndex != endIndex; ++startIndex) {
							Core::Individual parent1 =
								population.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));
							Core::Individual parent2 =
								population.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));

							std::lock_guard<std::mutex> lg(protectWriteMutex);
							newPopulation.push_back(parent1.Mate(parent2));
						}
					}
				);

				startIndex = endIndex;
			}

			// Optimization: have main thread process what's left
			for (; startIndex != newPopulation.capacity(); startIndex++) {
				Core::Individual parent1 =
					population.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));
				Core::Individual parent2 =
					population.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));

				std::lock_guard<std::mutex> lg(protectWriteMutex);
				newPopulation.push_back(parent1.Mate(parent2));
			}

			// Wait for threads to finish
			std::for_each(threadsManager.begin(), threadsManager.end(), [](std::thread& t) { t.join(); });

			// End details log here
			if (verbose)
				spdlog::info("=========================================\n");

			// Update current population with new
			population = std::move(newPopulation);
			generation += 1;
		} while (true);

		return fitnessHistory;
	}

	void BasicMating() const {
		Core::Individual person1{ Core::Individual::CreateGenome(TARGET), TARGET };
		Core::Individual person2{ Core::Individual::CreateGenome(TARGET), TARGET };

		Core::Individual person3 = person1.Mate(person2);

		spdlog::info("Parent 1: {}", person1.GetDetails());
		spdlog::info("Parent 2: {}", person2.GetDetails());
		spdlog::info("Child 1*: {}\n\n", person3.GetDetails());
	}

private:
	std::string TARGET = "THIS IS THE WAY";

	// Available processing cores
	int NUM_AVAILABLE_CORES = std::thread::hardware_concurrency();
	// Minimum elments per thread
	int MIN_PER_THREAD = 50;

	std::mutex protectWriteMutex;

	// Helper function for thread to process a block of data
	std::function<
		void(
			size_t, size_t,
			std::vector<Core::Individual>&, std::vector <Core::Individual>&,
			size_t&
		)
	> ProcessBlock = [&](
		size_t currentIndex, size_t endIndex,
		std::vector<Core::Individual>& currentPop, std::vector <Core::Individual>& newPop,
		size_t& matePoolSize
	) {
		// Do mating
		for (; currentIndex != endIndex; currentIndex++) {
			// Get a random index within the range of 50% of the populace and use to retrieve a parent
			Core::Individual parent1 = currentPop.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));
			Core::Individual parent2 = currentPop.at(Core::RandomNumberGenerator::GetInstance().Generate(0, matePoolSize - 1));

			std::cout << "Parent 1: " << parent1.GetDetails() << '\n';
			std::cout << "Parent 2: " << parent2.GetDetails() << '\n';

			// Protect the write operation if needed
			newPop[currentIndex] = parent1.Mate(parent2);
		}
	};
};

//DECLARE_MAIN(GAApplication)

