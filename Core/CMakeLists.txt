# ============================================================
#  Core/CMakeLists.txt
#  Builds the Core as a STATIC library called "Core".
#
#  App code links against this target. The Core knows
#  nothing about App — dependency flows one way only.
# ============================================================

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Build Core as a static library from those source files.
add_library(Core STATIC ${CORE_SOURCES})

# ── Include paths for Core ───────────────────────────────
# PUBLIC: Core's own code AND any target linking to Core
#         can #include files relative to Core/*.
#         e.g. #include "Core/Application.h" works from anywhere.
target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# ── Link dependencies ──────────────────────────────────────
# target_link_libraries wires up dependencies.
#
# PUBLIC scope here means: "Core needs these, and so does
# anything that links to Core." App will automatically get
# spdlog on its include/link path just by linking to Core
target_link_libraries(Core PUBLIC
    spdlog::spdlog	#logging			(FetchContent target, note :: namespace)
)
