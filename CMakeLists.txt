cmake_minimum_required(VERSION 3.22)
project(Y2KaoZNetwork VERSION 0.0.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

add_library(${PROJECT_NAME} 
    "include/Y2KaoZ/Network/Buffers/NotEnoughData.hpp"
    "include/Y2KaoZ/Network/Buffers/NotEnoughSpace.hpp"
    "include/Y2KaoZ/Network/Buffers/ReferenceCounted.hpp"
    "include/Y2KaoZ/Network/Buffers/SpanBufferReader.hpp"
    "include/Y2KaoZ/Network/Buffers/SpanBufferWriter.hpp"
    "include/Y2KaoZ/Network/Buffers/VectBufferWriter.hpp"
    "include/Y2KaoZ/Network/Concepts/TriviallyCopyableStandardLayout.hpp"
    "include/Y2KaoZ/Network/Tcp/Acceptor.hpp"
    "src/Y2KaoZ/Network/Buffers/NotEnoughData.cpp"
    "src/Y2KaoZ/Network/Buffers/NotEnoughSpace.cpp"
    "src/Y2KaoZ/Network/Buffers/ReferenceCounted.cpp"
    "src/Y2KaoZ/Network/Buffers/SpanBufferReader.cpp"
    "src/Y2KaoZ/Network/Buffers/SpanBufferWriter.cpp"
    "src/Y2KaoZ/Network/Buffers/VectBufferWriter.cpp"
    "src/Y2KaoZ/Network/Tcp/Acceptor.cpp"
)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}/include"
)
# target_link_libraries(${PROJECT_NAME} PUBLIC )

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "include/Y2KaoZ/Network/Visibility.hpp")

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" "${CMAKE_CURRENT_BINARY_DIR}/include/" TYPE INCLUDE)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

add_subdirectory("./tests")

configure_file(PKGBUILD.in PKGBUILD @ONLY)