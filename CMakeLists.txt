#set CMAKE version
cmake_minimum_required (VERSION 3.10)

#set C version (required for auto)
set(CMAKE_C_STANDARD 26)
set(CMAKE_CXX_STANDARD 26)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set the project name
project ("cpp-riemann-sum-approximation")

# Add the src directory
add_subdirectory ("src")


