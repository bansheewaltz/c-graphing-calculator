find_package(Check REQUIRED)
find_program(CHECKMK checkmk)

file(GLOB SUITES_CHECK
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.check
)

set(SUITES_EXE)

foreach(suite_check ${SUITES_CHECK})
    get_filename_component(suite_name ${suite_check} NAME_WLE)
    string(PREPEND suite_name "test_")
    string(CONCAT suite_c "${CMAKE_CURRENT_LIST_DIR}/" "${suite_name}.c")
    add_custom_command(
        OUTPUT ${suite_c}
        COMMAND ${CHECKMK} ${suite_check} > ${suite_c}
        DEPENDS ${suite_check}
        VERBATIM
    )
    add_executable(${suite_name} ${suite_c})
    list(APPEND SUITES_EXE ${suite_name})
    target_link_libraries(${suite_name} ${LIB_NAME} check)
    add_test(${suite_name} ${suite_name})
endforeach()

add_custom_target(run_tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND}
        --output-on-failure
        --rerun-failed
    || exit 0
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "You can find test logs at ${CMAKE_CURRENT_BINARY_DIR}/Testing/Temporary/LastTest.log"
    DEPENDS ${SUITES_EXE}
)

find_program(GCOVR_EXECUTABLE gcovr)

if(ENABLE_COVERAGE AND GCOVR_EXECUTABLE)
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
elseif(ENABLE_COVERAGE AND NOT GCOVR_EXECUTABLE)
    set(ENABLE_COVERAGE OFF)
    message(WARNING "gcovr program required for code coverage analysis was not found.")
endif()
