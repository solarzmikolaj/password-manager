cmake_minimum_required(VERSION 3.24)
project(PJCOSTATECZNY)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY      https://github.com/fmtlib/fmt
        GIT_TAG             a33701196adfad74917046096bf5a2aa0ab0bb50
)
FetchContent_MakeAvailable(fmt)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(PJCOSTATECZNY main.cpp Menus.cpp Menus.h PasswordGenerator.cpp PasswordGenerator.h Record.cpp Record.h OperationsOnFile.cpp OperationsOnFile.h OperationOnDatabase.cpp OperationOnDatabase.h)
target_link_libraries(PJCOSTATECZNY fmt)