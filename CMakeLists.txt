cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "gcc")
project(smartcalc)

# specify the output binaries' names
set(APP_NAME ${PROJECT_NAME})
set(LIB_NAME libsmartcalc)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(ENABLE_DOCS "Build documenation" ON)
option(ENABLE_TESTING "Enable testing" ON)
option(ENABLE_COVERAGE "Enable code coverage" ON)

if(ENABLE_TESTING)
  include(CTest)
endif()

add_subdirectory(src/lib)
add_subdirectory(src/app)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# dist target
include(InstallRequiredSystemLibraries)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-v1.0-Source")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v1.0-${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
