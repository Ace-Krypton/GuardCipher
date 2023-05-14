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

add_executable(GuardCipher src/main.cpp src/Categories.cpp include/Categories.hpp src/Menu.cpp include/Menu.hpp
        src/Passwords.cpp include/Passwords.hpp)
target_link_libraries(GuardCipher fmt::fmt)
