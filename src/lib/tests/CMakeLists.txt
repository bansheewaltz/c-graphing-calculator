find_package(Check REQUIRED)
find_program(CHECKMK checkmk)

file(GLOB SUITES_CHECK
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.check
)

set(SUITES_EXE)

foreach(suite_check ${SUITES_CHECK})
    string(REPLACE ".check" ".c" suite_c ${suite_check})
    add_custom_command(
        OUTPUT ${suite_c}
        COMMAND ${CHECKMK} ${suite_check} > ${suite_c}
        DEPENDS ${suite_check}
        VERBATIM
    )
    get_filename_component(exe ${suite_check} NAME_WLE)
    add_executable(${exe} ${suite_c})
    list(APPEND SUITES_EXE ${exe})
    target_link_libraries(${exe} ${LIB_NAME} check)
    add_test(${exe} ${exe})
endforeach()

add_custom_target(run_tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND}
        --output-on-failure
        --rerun-failed
    DEPENDS ${SUITES_EXE}
)

find_program(GCOVR_EXECUTABLE gcovr)

if(LIBSMARTCALC_COVERAGE AND GCOVR_EXECUTABLE)
    message(STATUS "Code coverage is enabled")
    set(LIBSMARTCALC_COVERAGE_DIR ${PROJECT_BINARY_DIR}/coverage)
    target_compile_options(libsmartcalc PRIVATE --coverage)
    target_link_options(libsmartcalc PRIVATE --coverage)
    target_link_libraries(libsmartcalc gcov)

    add_custom_target(coverage ALL
        COMMAND ${GCOVR_EXECUTABLE}
            --txt
            --html-details=${LIBSMARTCALC_COVERAGE_DIR}/report.html
            --html-self-contained
            --decisions
            --root=${PROJECT_SOURCE_DIR}/
            --filter=${PROJECT_SOURCE_DIR}/include
            --filter=${PROJECT_SOURCE_DIR}/src
            --object-directory=${PROJECT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "You can find html coverage report at ${LIBSMARTCALC_COVERAGE_DIR}/report.html"
        DEPENDS run_tests create_coverage_dir
    )
    add_custom_target(create_coverage_dir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBSMARTCALC_COVERAGE_DIR})
elseif(LIBSMARTCALC_COVERAGE AND NOT GCOVR_EXECUTABLE)
    set(LIBSMARTCALC_COVERAGE OFF)
    message(WARNING "gcovr program required for code coverage analysis was not found.")
endif()
