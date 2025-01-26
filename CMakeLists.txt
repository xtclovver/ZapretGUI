cmake_minimum_required(VERSION 3.30)
project(ZapretGUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:\\CompilerKits\\Qt6\\6.8.1\\mingw_64")

find_package(Qt6 COMPONENTS
  Core
  Gui
  Widgets
  Test
  REQUIRED)

# Main library sources
set(LIB_SOURCES
    mainwindow.cpp
    configeditordialog.cpp
)

set(LIB_HEADERS
    mainwindow.h
    configeditordialog.h
)

set(LIB_UI
    mainwindow.ui
    configeditordialog.ui
)

# Create a library for shared code
add_library(ZapretGUILib
    ${LIB_SOURCES}
    ${LIB_HEADERS}
    ${LIB_UI}
)

target_link_libraries(ZapretGUILib
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
)

target_include_directories(ZapretGUILib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Main executable
add_executable(ZapretGUI
    main.cpp
)

target_link_libraries(ZapretGUI
  ZapretGUILib
)

# Enable testing and add tests subdirectory
enable_testing()
add_subdirectory(tests)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif()
    endif()
    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()
    foreach(QT_LIB Core Gui Widgets )
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach(QT_LIB)
endif()
