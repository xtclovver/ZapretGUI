# tests/CMakeLists.txt

# Test for MainWindow
add_executable(TestMainWindow
    test_mainwindow.cpp
)

target_link_libraries(TestMainWindow
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Test
    ZapretGUILib
)

target_include_directories(TestMainWindow PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(TestMainWindow PROPERTIES AUTOMOC ON)

add_test(NAME MainWindowTest COMMAND TestMainWindow)

# Test for ConfigEditorDialog
add_executable(TestConfigEditorDialog
    test_configeditordialog.cpp
)

target_link_libraries(TestConfigEditorDialog
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Test
    ZapretGUILib
)

target_include_directories(TestConfigEditorDialog PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(TestConfigEditorDialog PROPERTIES AUTOMOC ON)

add_test(NAME ConfigEditorDialogTest COMMAND TestConfigEditorDialog)
