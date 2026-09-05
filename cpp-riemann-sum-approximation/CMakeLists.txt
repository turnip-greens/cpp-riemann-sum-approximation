# CMakeList.txt : CMake project for cpp-riemann-sum-approximation, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (cpp-riemann-sum-approximation "main.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cpp-riemann-sum-approximation PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
