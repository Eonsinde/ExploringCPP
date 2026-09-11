# Minimum CMake version we need. 3.20+ has Fetchcontent improvements.
cmake_minimum_required(VERSION 3.20)

# Name the project and tell CMake we're using C++.
# VERSION IS optional metadata. LANGUAGes tells the complier toolchain to setup.
project(LearningCPP VERSION 1.0 LANGUAGES C CXX)

# Only generate Debug and Release configurations
# Use semicolon-separated list for multi-config generators (Visual Studio expects this)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# ── C++ Standard ──────────────────────────────────────────
# Require C++20 globally. "REQUIRED" means fail loudly if the
# compiler doesn't support it, rather than silently downgrading.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't allow compiler-specific extensions (keeps code portable)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Output directories ─────────────────────────────────────
# Put all compiled binaries in build/bin so they're easy to find.
# CMAKE_BINARY_DIR is wherever you ran `cmake -B <here>`.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ── FetchContent setup ─────────────────────────────────────
# FetchContent downloads and builds dependencies at configure time.
# Think of it as a built-in package manager, no install step needed.
include(FetchContent)

# -- spdlog (fast logging) --
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG        v1.15.1
)

# This single call actually downloads + configures all declared dependencies above.
# CMake is smart: if already downloaded, it skips the network step.
FetchContent_MakeAvailable(spdlog)

# Enable folder grouping in Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ── Subdirectories ─────────────────────────────────────────
# add_subdirectory tells CMake to go read that folder's CMakeLists.txt.
# ORDER MATTERS: Vendor before Core as Core links to Vendor's content
# add_subdirectory(Vendor)
add_subdirectory(Core)
add_subdirectory(App)

# Group vendor/dependency targets into a single folder
set_target_properties(spdlog PROPERTIES FOLDER "Vendor")