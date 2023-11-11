qt_add_executable(${APP_NAME}
    main.cpp
    mainwindow.cpp mainwindow.hpp mainwindow.ui
    qcustomplot.cpp qcustomplot.h
)

target_include_directories(${APP_NAME} PRIVATE .)

set_target_properties(${APP_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
