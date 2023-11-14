find_package(Check)
find_program(CHECKMK checkmk)

set(TEST_SUITES
  suites/infix_evaluate.check
  suites/infix_to_postfix.check
)

# foreach(test_suite ${TEST_SUITES})
# get_filename_component(file_c ${test_suite} NAME_WE)
# set(file_c "${file_c}.c")
# add_custom_command(
# OUTPUT suites/${file_c}
# COMMAND checkmk ${test_suite} > suites/${file_c}
# DEPENDS ${test_suite}
# VERBATIM
# )
# endforeach()

# add_custom_command(
# OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/infix_evaluate.c
# COMMAND checkmk suites/infix_evaluate.check > infix_evaluate.c
# DEPENDS suites/infix_evaluate.check
# VERBATIM
# )

file(GLOB CHECK_FILES ${CMAKE_CURRENT_LIST_DIR}/suites/*.check)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/suites/tests.c
    COMMAND ${CHECKMK} ${CHECK_FILES} > ${CMAKE_CURRENT_LIST_DIR}/suites/tests.c
    DEPENDS ${CHECK_FILES}
    VERBATIM
)
add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/suites/tests.c)
target_link_libraries(tests libsmartcalc check)

#add_executable(infix_evaluate ${CMAKE_CURRENT_SOURCE_DIR}/infix_evaluate.c)
#target_link_libraries(infix_evaluate libsmartcalc check)
