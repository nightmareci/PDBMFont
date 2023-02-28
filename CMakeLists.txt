cmake_minimum_required(VERSION 3.10)

project(BMFontTest VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(BMFontTest
	BMFontTest.cpp
	TextRenderer.cpp
	tinyxml2.cpp
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(BMFontTest PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)
if(TARGET SDL2::SDL2main)
	target_link_libraries(BMFontTest PRIVATE SDL2::SDL2main)
endif()
