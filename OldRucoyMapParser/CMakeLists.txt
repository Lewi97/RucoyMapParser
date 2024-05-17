# CMakeList.txt : CMake project for OldRucoyMapParser, include source and define
# project specific logic here.
#

project(RucoyMapParser)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "main.cpp"  "Parser.h" "Parser.cpp" "ByteStream.h" "Parser115.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
endif()

target_include_directories(${PROJECT_NAME}
	PRIVATE ../ext/raylib/include
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE raylib
)
