find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp src/mainwindow.hpp forms/mainwindow.ui
    third-party/qcustomplot.cpp third-party/qcustomplot.h
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

qt6_wrap_ui(UI_HEADERS
    forms/mainwindow.ui
)

target_include_directories(${APP_NAME}
    PRIVATE
        third-party
        ${CMAKE_CURRENT_BINARY_DIR}
)

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

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Target install
qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
