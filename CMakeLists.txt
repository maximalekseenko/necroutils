cmake_minimum_required(VERSION 3.20)

project(necroutils
    VERSION 2.0.0
    DESCRIPTION "Necromax's toolbox library"
    LANGUAGES CXX
)

# -------------------- OPTIONS --------------------
include(CMakeDependentOption)

option(NECROUTILS_INSTALL "Install necroutils" ${PROJECT_IS_TOP_LEVEL})

cmake_dependent_option(NECROUTILS_INSTALL_RUNCPPLINT "Install cpplint cmake tool" ON NECROUTILS_INSTALL OFF)

option(NECROUTILS_TESTING "Build testing suits" ${PROJECT_IS_TOP_LEVEL})

cmake_dependent_option(NECROUTILS_TESTING_GTEST "Create gtest suits" ON NECROUTILS_TESTING OFF)

cmake_dependent_option(NECROUTILS_TESTING_CPPLINT "Create cpplint suit" ON NECROUTILS_TESTING OFF)

# -------------------- OPTIONS --------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if(NOT PROJECT_IS_TOP_LEVEL)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endif()

# -------------------- LIBRARY TARGET --------------------
add_library(necroutils SHARED)
add_library(necroutils::necroutils ALIAS necroutils)

set_target_properties(necroutils PROPERTIES
    OUTPUT_NAME "necroutils"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# -------------------- COMPILE AND LINKING FLAGS --------------------
target_compile_features(necroutils PUBLIC cxx_std_20)

target_compile_definitions(necroutils PRIVATE NECROUTILS_LOCAL)

if(MSVC)
    target_compile_options(necroutils INTERFACE
        /W4
        /permissive-
    )
else()
    target_compile_options(necroutils INTERFACE
        -W
        -Wall 
        -Wcast-qual
        -Wextra
        -Wsign-compare
        -Wundef
        -Wpointer-arith
        -Wwrite-strings
        -Wredundant-decls
        -Wformat-security
        -Wformat=2
        -Winit-self
        -Wnon-virtual-dtor
        -Wsuggest-override
    )
endif()

if(MINGW)
    target_link_options(necroutils INTERFACE -static -static-libgcc -static-libstdc++)
endif()

# -------------------- DIRECTORIES AND SOURCES --------------------
target_include_directories(necroutils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(necroutils
    PRIVATE
        src/necroutils/colexc/collective_exception.cpp
        src/necroutils/logger/logger.cpp
        src/necroutils/logger/logsink.cpp
)

# -------------------- TESTING --------------------
if(NECROUTILS_TESTING_GTEST)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

if(NECROUTILS_TESTING_CPPLINT)
    include(necroutilsRunCpplint)
    necroutils_RunCpplint(necroutils 
        DIRECTORIES 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endif()

# -------------------- INSTALLATION --------------------
include(GNUInstallDirs)

set(NECROUTILS_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/cmake/necroutils")
set(NECROUTILS_EXPORT_TARGETS necroutilsTargets)

if(NECROUTILS_INSTALL_RUNCPPLINT)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/necroutilsRunCpplint.cmake
        DESTINATION ${NECROUTILS_INSTALL_LIBDIR}
    )
endif()

if (NECROUTILS_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS necroutils
        EXPORT ${NECROUTILS_EXPORT_TARGETS}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(EXPORT ${NECROUTILS_EXPORT_TARGETS}
        FILE ${NECROUTILS_EXPORT_TARGETS}.cmake
        NAMESPACE necroutils::
        DESTINATION ${NECROUTILS_INSTALL_LIBDIR}
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/necroutilsConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfig.cmake
        INSTALL_DESTINATION ${NECROUTILS_INSTALL_LIBDIR}
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/necroutilsConfigVersion.cmake
        DESTINATION ${NECROUTILS_INSTALL_LIBDIR}
    )
endif()
