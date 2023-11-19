find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.hpp mainwindow.ui
    qcustomplot.cpp qcustomplot.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(untitled
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(${APP_NAME} PRIVATE .)

set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER smartcalc.bansheewaltz
    MACOSX_BUNDLE ON
    WIN32_EXECUTABLE ON
)

target_link_libraries(${APP_NAME} PRIVATE
  ${LIB_NAME}
  Qt6::Widgets
  Qt6::PrintSupport
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
