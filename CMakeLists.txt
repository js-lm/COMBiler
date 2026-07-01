cmake_minimum_required(VERSION 3.13)

set(PROJECT_TARGET COMBiler)
project(${PROJECT_TARGET} VERSION 0.2 LANGUAGES CXX)

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

FetchContent_Declare(
	tinysoundfont
	GIT_REPOSITORY https://github.com/schellingb/TinySoundFont
	GIT_TAG main
)

FetchContent_MakeAvailable(raylib raygui magic_enum fmt tinysoundfont)

file(GLOB_RECURSE PROJECT_SOURCES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
)

add_executable(${PROJECT_TARGET} ${PROJECT_SOURCES})

file(GLOB ASSET_ENTRIES 
     CONFIGURE_DEPENDS 
     LIST_DIRECTORIES true 
     "${CMAKE_CURRENT_LIST_DIR}/assets/*"
)

target_compile_definitions(${PROJECT_TARGET} PRIVATE
	$<$<CONFIG:Debug>:DEBUG>
	COMBILER_VERSION="${PROJECT_VERSION}"
)

target_include_directories(${PROJECT_TARGET} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/sources
	${raygui_SOURCE_DIR}/src
	${tinysoundfont_SOURCE_DIR}
)

target_link_libraries(${PROJECT_TARGET}
	raylib
	magic_enum::magic_enum
	fmt::fmt
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set_target_properties(${PROJECT_TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/release"
	)

	add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_CURRENT_LIST_DIR}/assets"
			"$<TARGET_FILE_DIR:${PROJECT_TARGET}>"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${CMAKE_CURRENT_LIST_DIR}/LICENSE"
			"$<TARGET_FILE_DIR:${PROJECT_TARGET}>/LICENSE"
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_CURRENT_LIST_DIR}/licenses"
			"$<TARGET_FILE_DIR:${PROJECT_TARGET}>/licenses"
	)
else()
	foreach(asset_entry IN LISTS ASSET_ENTRIES)
		get_filename_component(asset_name "${asset_entry}" NAME)
		add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:${PROJECT_TARGET}>/${asset_name}"
			COMMAND ${CMAKE_COMMAND} -E create_symlink
				"${asset_entry}"
				"$<TARGET_FILE_DIR:${PROJECT_TARGET}>/${asset_name}"
		)
	endforeach()
endif()
