cmake_minimum_required(VERSION 3.16)

# set library name if it is not already specified from top-level
get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

if(NOT IS_SUBPROJECT)
  set(LIB_NAME libsmartcalc)
endif()

project(${LIB_NAME} LANGUAGES C)

# specify the languages' standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(${LIB_NAME} STATIC
  include/smartcalc.h

  src/grammar.c src/grammar.h
  src/error_handling.c

  src/data_structures/token.h
  src/data_structures/tkn_queuearr.c src/data_structures/tkn_queuearr.h
  src/data_structures/tkn_linkedl.c src/data_structures/tkn_linkedl.h

  src/utils/stringutils.c src/utils/stringutils.h

  src/processing/expression_handler.c
  src/processing/tokenization.c src/processing/tokenization.h
  src/processing/postfix_evaluation.c src/processing/postfix_evaluation.h
  src/processing/infix_to_postfix.c src/processing/infix_to_postfix.h
)

target_include_directories(${LIB_NAME} PUBLIC include)
target_include_directories(${LIB_NAME} PRIVATE
  src
  src/processing
  src/data_structures
  src/utils
)

# Tests
