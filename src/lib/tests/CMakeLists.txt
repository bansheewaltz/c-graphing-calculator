find_package(Check REQUIRED)
find_program(CHECKMK checkmk)

file(GLOB SUITES_CHECK
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/*.check
)

set(EXES)

foreach(suite_check ${SUITES_CHECK})
    string(REPLACE ".check" ".c" suite_c ${suite_check})
    add_custom_command(
        OUTPUT ${suite_c}
        COMMAND ${CHECKMK} ${suite_check} > ${suite_c}
        DEPENDS ${suite_check}
        VERBATIM)
    get_filename_component(exe ${suite_check} NAME_WLE)
    add_executable(${exe} ${suite_c})
    list(APPEND EXES ${exe})
    target_link_libraries(${exe} ${LIB_NAME} check)
    add_test(${exe} ${exe})
endforeach()

add_custom_target(run_tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --rerun-failed
    DEPENDS ${EXES}
)
