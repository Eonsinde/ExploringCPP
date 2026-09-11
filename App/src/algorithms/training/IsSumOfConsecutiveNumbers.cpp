#include "EntryPoint/Application.h"


class IsSumOfConsecutiveNumbersApp : public Core::Application
{
public:
    virtual void Run() override {
        std::cout << "========== Naive Approach ==========\n";
        spdlog::info("24 is sum of consecutive numbers: {}", NaiveApproach(24));
        spdlog::info("8 is sum of consecutive numbers: {}", NaiveApproach(8));
        spdlog::info("19 is sum of consecutive numbers: {}", NaiveApproach(19));

        std::cout << "\n========== Best Approach ==========\n";
        spdlog::info("24 is sum of consecutive numbers: {}", BestApproach(24));
        spdlog::info("8 is sum of consecutive numbers: {}", BestApproach(8));
        spdlog::info("19 is sum of consecutive numbers: {}", BestApproach(19));
    }

    // O(n)
    bool NaiveApproach(int n) {
        // The main loop to that controls the starting point for the summation of consecutive numbers
        // Start: 1; Next: 2; Next: 3; Next: 4
        for (int i = 0; i < n; i++) {
            // Reset sum each iteration
            int sum = 0;

            // Start: 1 + 2 + 3; Next: 2 + 3 + 4; Next: 3 + 4 + 5
            for (int j = i; j < n; j++) {
                sum += j;

                // j > i: ensures the it was more that one number that attained the sum equal to n.
                // This way, sum will always be the addition of two or more consecutive numbers and not one.
                // Peradventure, j == i, and sum = j, and sum = n, then it wasn't the sum of two or more 
                // consecutive integers that helped reached n, it was just one number, hence, invalid
                if (sum == n && j > i)
                    return true;

                // If greater than n, break out
                if (sum > n)
                    break;
            }
        }

        return false;
    }

    // O(1)
    bool BestApproach(int n) {
        if (n == 1)
            return false;

        // If n can be expressed as a power of two, then it can't be derived by the
        // sum of two or more consecutive integers
        if ((n & (n - 1)) == 0) {
            return false;
        }

        return true;
    }
};

//DECLARE_MAIN(IsSumOfConsecutiveNumbersApp)