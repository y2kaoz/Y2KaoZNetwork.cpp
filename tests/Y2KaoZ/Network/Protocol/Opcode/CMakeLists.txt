find_package(Catch2 3 REQUIRED)

add_executable(OpcodeTests
    BaseOpcode.cpp
    MajorPolicyT.cpp
    MinorPolicyT.cpp
)

target_link_libraries(OpcodeTests 
    PRIVATE 
        Catch2::Catch2WithMain
        ${PROJECT_NAME}
)

add_test( NAME OpcodeTests COMMAND OpcodeTests )