cmake_minimum_required(VERSION 3.16)

project(MarbleSolitaire VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)   # Runs moc on anything containing Q_OBJECT.
set(CMAKE_AUTORCC ON)   # Compiles .qrc resource files.
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Game rules, with no Qt dependency, so the tests can link it alone ---
add_library(tic_tac_toe_game STATIC
    src/tic_tac_toe_game.cc
    src/tic_tac_toe_game.h
)
target_include_directories(tic_tac_toe_game PUBLIC src)

# --- The GUI application ---
add_executable(MarbleSolitaire
    src/main.cc
    src/main_window.cc
    src/main_window.h
    src/tic_tac_toe_widget.cc
    src/tic_tac_toe_widget.h
    assets/assets.qrc
)
target_link_libraries(MarbleSolitaire PRIVATE tic_tac_toe_game Qt6::Widgets)

if(WIN32)
    set_target_properties(MarbleSolitaire PROPERTIES WIN32_EXECUTABLE ON)
endif()

# --- Tests ---
enable_testing()
add_subdirectory(tests)
