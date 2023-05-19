cmake_minimum_required(VERSION 3.25)
project(GuardCipher)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(fmt)

add_executable(GuardCipher src/main.cpp src/categories.cpp include/categories.hpp src/menu.cpp include/menu.hpp
        src/passwords.cpp include/passwords.hpp src/encrypt.cpp include/encrypt.hpp)
target_link_libraries(GuardCipher fmt::fmt)
