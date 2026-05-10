cmake_minimum_required(VERSION 3.20)

# -------------------- PROJECT --------------------
project(necroutils
    VERSION 1.0.0
    DESCRIPTION "Necromax's toolbox library"
    LANGUAGES CXX
)

# -------------------- OPTIONS --------------------
if(NOT OUT_NAME)
    set(OUT_NAME "necroutils")
endif()
option(RUN_CPPLINT "Run cpplint (Google style tests) when building" ON)

# -------------------- LIBRARY TARGET --------------------
add_library(necroutils SHARED)
add_library(necroutils::necroutils ALIAS necroutils)

set_target_properties(necroutils PROPERTIES
    OUTPUT_NAME ${OUT_NAME}
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# -------------------- C++ STANDARD & WARNINGS --------------------
target_compile_features(necroutils PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(necroutils PRIVATE /W4 /permissive-)
else()
    target_compile_options(necroutils PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor)
endif()

# -------------------- INCLUDE DIRECTORIES --------------------
target_include_directories(necroutils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# -------------------- COMPILE DEFINITIONS --------------------
target_compile_definitions(necroutils
    PRIVATE nECROuTILS_LOCAL
)

# -------------------- SOURCES --------------------
target_sources(necroutils
    PRIVATE
        src/necroutils/colexc/collective_exception.cpp
        src/necroutils/logger/logger.cpp
        src/necroutils/logger/logsink.cpp
)

# -------------------- OPTIONAL CPPLINT --------------------
if(RUN_CPPLINT)
    include(FetchContent)
    FetchContent_Declare(
        cpplint
        GIT_REPOSITORY https://github.com/cpplint/cpplint/
        GIT_TAG        2.0.2)
    FetchContent_MakeAvailable(cpplint)

    message(!!! ${cpplint_SOURCE_DIR})
    find_package(Python3 COMPONENTS Interpreter REQUIRED)

    add_custom_command(
        TARGET necroutils POST_BUILD
        COMMAND ${Python3_EXECUTABLE}
        ${cpplint_SOURCE_DIR}/cpplint.py
        "--recursive"
        "--quiet"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cpplint (Google styles tests)"
    )
endif()

# -------------------- INSTALLATION --------------------
# Install library
install(TARGETS necroutils
    EXPORT necroutilsTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/ DESTINATION include)

# Export for use with find_package
install(EXPORT necroutilsTargets
    FILE necroutilsTargets.cmake
    NAMESPACE necroutils::
    DESTINATION lib/cmake/necroutils
)

# -------------------- CONFIG PACKAGE --------------------
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/necroutils-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfig.cmake"
    INSTALL_DESTINATION lib/cmake/necroutils
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfigVersion.cmake"
    DESTINATION lib/cmake/necroutils
)