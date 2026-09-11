#include "EntryPoint/Application.h"

#include <thread>
#include <mutex>
#include <chrono>


// This example showcases a good use case of acquiring multiple mutexes
// in a thread using a simple real life example of a Banking system.
// In this example use concurrency to have money be transferred from one
// account to the other, while also updating a user's name

struct BankAccount
{
    class TransactionException : public std::exception {
    public:
        TransactionException(const std::string& errorText) : _mMessage{ errorText } {};

        virtual const char* what() const {
            return _mMessage.c_str();
        }

    private:
        std::string _mMessage;
    };

    friend void TransferMoney(BankAccount& from, BankAccount& to, int amount, bool& outStatus) {
        // Acquire the locks in the order: from -> to
        std::lock(from._mMutex, to._mMutex);

        std::lock_guard<std::mutex> LG1(from._mMutex, std::adopt_lock);
        std::lock_guard<std::mutex> LG2(to._mMutex, std::adopt_lock);

        // Ensure amount doesn't exceed from's balance
        if (amount > from.mBalance)
            throw TransactionException("Insufficient Funds!");

        // Perform transfer logic here
        int fromTempBalance = from.mBalance;
        // Remove the amount to be transferred
        fromTempBalance -= amount;
        // Update to's account balance
        to.mBalance += amount;
        // Finally, update from's balance
        from.mBalance = fromTempBalance;

        // We assume all went well and return true
        outStatus = true;
    }

    BankAccount(const std::string& identifier, const int value)
        : mName{ identifier }, mBalance{ value } {
    }

    void DisplayBalance() {
        spdlog::info("Your account balance, {}, is {}", mName, mBalance);
    }

    bool UpdateName(const std::string& newName) {
        std::lock_guard<std::mutex> LG(_mMutex);

        std::this_thread::sleep_for(std::chrono::seconds(5));

        mName = newName;

        // We assume the update operation always goes fine and return true
        return true;
    }

    std::string mName;
    int mBalance;

private:
    std::mutex _mMutex;
};


class DeadlockCircumvention4 : public Core::Application
{
public:
    virtual void Run() {
        std::cout << "========== Opening Balance ==========\n";
        user1.DisplayBalance();
        user2.DisplayBalance();

        // Perform transfer operation here
        bool transferResult = 0, nameUpdateResult = 0;

        // This thread performs the transfer operation from user1 to 2
        std::thread t1(TransferMoney, std::ref(user1), std::ref(user2), 10000, std::ref(transferResult));

        // This thread performs the name update to user2
        std::thread t2(
            [&]() {
                nameUpdateResult = user2.UpdateName("Jesutise Olasinde");
            }
        );

        t1.join();
        t2.join();

        std::cout << "\n\n========== Operations Result ==========\n";
        spdlog::info("Transfer Result: {}", transferResult);
        spdlog::info("Name Update Result: {}", nameUpdateResult);

        std::cout << "\n\n========== Closing Balance ==========\n";
        user1.DisplayBalance();
        user2.DisplayBalance();
    }

private:
    BankAccount user1{ "Temi Bolade", 87000 };
    BankAccount user2{ "Enoch Olasinde", 50000 };
};


//DECLARE_MAIN(DeadlockCircumvention4)
