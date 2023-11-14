cmake_minimum_required(VERSION 3.16)

project(smartcalc)
find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport)

# specify the ouput directory
set(OUTPUT_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINARY_DIR}) # exe
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BINARY_DIR}/lib) # static libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BINARY_DIR}/lib) # shared libraries

# specify the output binaries' names
set(APP_NAME ${PROJECT_NAME})
set(LIB_NAME libsmartcalc)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CTest)
add_subdirectory(src/lib)

# qt_standard_project_setup()
# add_subdirectory(src/app)

# target_link_libraries(${APP_NAME} PRIVATE
# ${LIB_NAME}
# Qt6::Widgets
# Qt6::PrintSupport
# )
