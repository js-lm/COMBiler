cmake_minimum_required(VERSION 3.13)

set(PROJECT_TARGET COMBiler)
project(${PROJECT_TARGET} CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib
	GIT_TAG 5.5
)

FetchContent_Declare(
	raygui
	GIT_REPOSITORY https://github.com/raysan5/raygui
	GIT_TAG 4.0
)

FetchContent_Declare(
	magic_enum
	GIT_REPOSITORY https://github.com/Neargye/magic_enum
	GIT_TAG v0.9.7
)

FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt
	GIT_TAG 12.1.0
)

FetchContent_MakeAvailable(raylib raygui magic_enum fmt)

file(GLOB_RECURSE PROJECT_SOURCES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
)

add_executable(${PROJECT_TARGET} ${PROJECT_SOURCES})

target_compile_definitions(${PROJECT_TARGET} PRIVATE
	$<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(${PROJECT_TARGET} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/sources
	${raygui_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_TARGET}
	raylib
	magic_enum::magic_enum
	fmt::fmt
)
