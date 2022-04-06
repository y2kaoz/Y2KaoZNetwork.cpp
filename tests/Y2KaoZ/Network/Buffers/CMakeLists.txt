find_package(Catch2 3 REQUIRED)

add_executable(BuffersTest
    Endian.cpp
)

target_link_libraries(BuffersTest 
    PRIVATE 
        Catch2::Catch2WithMain
        ${PROJECT_NAME}
)

add_test( NAME BuffersTest COMMAND BuffersTest )