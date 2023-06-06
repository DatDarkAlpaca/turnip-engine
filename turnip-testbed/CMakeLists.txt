cmake_minimum_required(VERSION 3.12)

project("testbed")

set(TESTBED_SOURCES "main.cpp")
add_executable (${PROJECT_NAME} ${TESTBED_SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC "TUR_IMPORT")

target_include_directories(${PROJECT_NAME} PRIVATE "turnip")
target_link_libraries(${PROJECT_NAME} PRIVATE "turnip")