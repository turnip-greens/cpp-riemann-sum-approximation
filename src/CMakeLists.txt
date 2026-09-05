
# compile c srcs
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
)


# make an executable from all source files
if (SRC_FILES)
  add_executable(cpp-riemann-sum-approximation ${SRC_FILES})
else()
  message(FATAL_ERROR "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}. Add main.cpp or other source files.")
endif()
