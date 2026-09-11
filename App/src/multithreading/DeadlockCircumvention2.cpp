#include "EntryPoint/Application.h"
#include "Classes/Threads/HierarchicalMutex.h"

#include <spdlog/spdlog.h>
#include <thread>
#include <mutex>
#include <string>


// This example shows expected usage of the HierarchicalMutex.
// The priority enforced is Highest to Lowest mutex locking, meaning
// if a lower layer mutex is locked, trying to acquire a higher level
// mutex would result in the hierarchy violation error
class DeadlockCircumvention2 : public Core::Application
{
public:
    virtual void Run() {
        // Using the main thread here for optimization.
        // This will result in hierarchy violation error
        DoOtherStuff();

        // This runs well
        std::thread t2(
            [this]() {
                DoMidWithLowLevelStuff();
            }
        );

        std::thread t3(
            [this]() {
                DoHighWithLowLevelStuff();
            }
        );

        t2.join();
        t3.join();
    }

    int DoLowLevelStuff() {
        std::lock_guard<HierarchicalMutex> LG(_mMutex3);

        spdlog::info("Processing low level stuff...");

        int value = 12 << 1;

        return value;
    }

    void DoMidWithLowLevelStuff() {
        // Acquire mid level mutex
        std::lock_guard<HierarchicalMutex> LG(_mMutex2);

        spdlog::info("Processing mid level stuff: {}", HierarchicalMutex::GetThisThreadHierarchyValue());

        // Do some low level stuff here
        //std::cout << "Result of low level stuff: " << DoLowLevelStuff() << '\n';
        spdlog::info("Result of low level stuff after mid level operation: {}", DoLowLevelStuff());
    }

    void DoHighWithLowLevelStuff() {
        // Acquire the highest layer's mutex
        std::lock_guard<HierarchicalMutex> LG(_mMutex1);
        
        spdlog::info("Processing high level stuff: {}", HierarchicalMutex::GetThisThreadHierarchyValue());

        // Also do low level stuff here which will acquire the mid layer mutex
        spdlog::info("Result of low level stuff after high level operation: {}", DoLowLevelStuff());
    }

    // This triggers the hierarchy violation error intentionally
    void DoOtherStuff() {
        try {
            // Acquire the lowest layer's mutex here
            std::lock_guard<HierarchicalMutex> LG(_mMutex3);

            // Tries to acquire a higher level mutex here
            DoHighWithLowLevelStuff();
        }
        catch (std::exception& e) {
            spdlog::error("{}", e.what());
        }
    }

private:
    // Highest layer
    HierarchicalMutex _mMutex1{ 5000 };
    // Mid layer
    HierarchicalMutex _mMutex2{ 2500 };
    // Lowest layer
    HierarchicalMutex _mMutex3{ 300 };
};

 //DECLARE_MAIN(DeadlockCircumvention2)