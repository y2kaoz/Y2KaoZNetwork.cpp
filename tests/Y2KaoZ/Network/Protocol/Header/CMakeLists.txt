find_package(Catch2 3 REQUIRED)

add_executable(HeaderTests
    BaseHeader.cpp
    BodySizePolicyT.cpp
    HeaderIdPolicyT.cpp
)

target_link_libraries(HeaderTests 
    PRIVATE 
        Catch2::Catch2WithMain
        ${PROJECT_NAME}
)

add_test( NAME HeaderTests COMMAND HeaderTests )