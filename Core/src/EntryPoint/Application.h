// Enrtry point

#include <iostream>
#include <spdlog/spdlog.h>

namespace Core {
    // Application: is the Root class which all application classes must inherit from.
    // Child class must override the Run() member function as this is what gets executed in main()
    class Application {
    public:
        Application();

        // Prevent copy
        Application(Application&) = delete;
        Application& operator=(Application&) = delete;

        virtual ~Application();

        // This is what gets executed by main() and must be overriden by child classes
        virtual void Run() = 0;

        // Return the running application instance
        static Application& Get();

    private:
        static inline Application* _sAppInstance = nullptr;
    };
}

#define DECLARE_MAIN(App)   \
    int main() {    \
        std::cout << "========== Application Name: " << #App << " ==========\n\n";   \
        App* instance = new App();   \
        instance->Run();    \
        delete instance;    \
        instance = nullptr; \
        return 0;   \
    }