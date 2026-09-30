# ============================================================
#  App/CMakeLists.txt
#  Builds the actual app as an executable.
#
#  This is intentionally thin — the app just links to Core
#  and gets everything it needs transitively.
# ============================================================

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# add_executable: produce a runnable binary (not a library) using the source files.
add_executable(MyApp ${APP_SOURCES})

# Link to Core. Because Core's dependencies are PUBLIC,
# MyApp automatically gets spdlog on its linker path
target_link_libraries(MyApp PRIVATE Core)


# Copy resources next to the executable every time you build
add_custom_command(TARGET MyApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
            $<TARGET_FILE_DIR:MyApp>/resources
    COMMENT "Copying resources to build directory"
)