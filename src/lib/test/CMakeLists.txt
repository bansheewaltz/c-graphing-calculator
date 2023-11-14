find_package(Check REQUIRED)
find_program(CHECKMK checkmk)

file(GLOB suites_check CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/suites/*.check)

foreach(suite_check ${suites_check})
    string(REPLACE ".check" ".c" suite_c ${suite_check})
    add_custom_command(
        OUTPUT ${suite_c}
        COMMAND ${CHECKMK} ${suite_check} > ${suite_c}
        DEPENDS ${suite_check}
        VERBATIM)
    get_filename_component(exe ${suite_check} NAME_WLE)
    add_executable(${exe} ${suite_c})
    target_link_libraries(${exe} libsmartcalc check)
    add_test(${exe} ${exe})
endforeach()
