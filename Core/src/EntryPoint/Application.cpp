#include "Application.h"

#include <stdexcept>


namespace Core {
    Application::Application() {
        // Set output console to use UTF-8 encoding
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        if (_sAppInstance) {
            throw std::runtime_error("Application instance is already running!");
        }

        _sAppInstance = this;
    }

    Application::~Application() {
        _sAppInstance = nullptr;
    }

    Application& Application::Get() {
        if (_sAppInstance == nullptr) {
            throw std::runtime_error("Application has not been instantiated yet!");
        }

        return *_sAppInstance;
    }
}