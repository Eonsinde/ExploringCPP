add_library(Vendor INTERFACE)

target_include_directories(Vendor INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(Vendor INTERFACE cxx_std_20)