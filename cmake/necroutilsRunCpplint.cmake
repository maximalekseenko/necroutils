function(necroutils_RunCpplint PROJECT_NAME)
    cmake_parse_arguments(PARSE_ARGV 1 NRC "" "" "DIRECTORIES")

    set(CPP_FILES "")
    foreach(dir IN LISTS NRC_DIRECTORIES)
        file(GLOB_RECURSE dir_cpp
            CONFIGURE_DEPENDS
            "${dir}/*.cpp"
            "${dir}/*.cc"
            "${dir}/*.cxx"
            "${dir}/*.c++"
            "${dir}/*.h"
            "${dir}/*.hpp"
            "${dir}/*.hh"
            "${dir}/*.hxx"
        )

        list(APPEND CPP_FILES ${dir_cpp})
    endforeach()

    set(LINT_TARGET "${PROJECT_NAME}_lint")

    add_custom_target(${LINT_TARGET} ALL DEPENDS ${CPP_FILES})

    find_package(Python3 COMPONENTS Interpreter)

    if(Python3_EXECUTABLE)
        set(CPPLINT_FILE "${CMAKE_BINARY_DIR}/cpplint2_0.py")
        if(NOT EXISTS "${CPPLINT_FILE}")
            file(DOWNLOAD
                "https://raw.githubusercontent.com/cpplint/cpplint/refs/tags/2.0.2/cpplint.py"
                "${CPPLINT_FILE}"
            )
        endif()

        add_custom_command(TARGET ${LINT_TARGET} POST_BUILD
            COMMAND ${Python3_EXECUTABLE}
            ${CPPLINT_FILE}
            "--recursive"
            "--quiet"
            "--root=${CMAKE_CURRENT_SOURCE_DIR}"
            "--filter=-build/include_order"
            ${NRC_DIRECTORIES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Running cpplint (Google styles tests)."
        )
    else()
        add_custom_command(
            TARGET ${LINT_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo_append "CMake Warning: Not running cpplint, python not found.\n"
        )
    endif()
endfunction()