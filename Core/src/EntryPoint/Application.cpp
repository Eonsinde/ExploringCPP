#include "Application.h"

#include <stdexcept>


namespace Core {
    Application::Application() {
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