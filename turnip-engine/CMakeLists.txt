add_executable (turnip-engine "turnip-engine.cpp" "turnip-engine.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET turnip-engine PROPERTY CXX_STANDARD 17)
endif()