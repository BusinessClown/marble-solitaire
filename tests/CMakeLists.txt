include(FetchContent)

# Downloads GoogleTest at configure time, so nothing has to be installed
# system-wide. Pin the tag; do not track a moving branch.
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# Keeps GoogleTest from forcing its own CRT setting on MSVC builds.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(tic_tac_toe_game_test tic_tac_toe_game_test.cc)

target_link_libraries(tic_tac_toe_game_test
    PRIVATE tic_tac_toe_game GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tic_tac_toe_game_test)
