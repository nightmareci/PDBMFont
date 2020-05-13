cmake_minimum_required(VERSION 3.10)

project(BMFontTest VERSION 0.0 LANGUAGES CXX)

if(${VCPKG_TOOLCHAIN})
    set(VCPKG_CRT_LINKAGE dynamic)
    set(VCPKG_LIBRARY_LINKAGE dynamic)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig)
if(${PKG_CONFIG_FOUND})
    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
    pkg_check_modules(SDL2_image REQUIRED IMPORTED_TARGET SDL2_image)
    set(LIBRARIES
    	${SDL2_LIBRARIES}
    	${SDL2_image_LIBRARIES}
		${CMAKE_DL_LIBS})
else()
    find_package(sdl2 REQUIRED)
    find_package(sdl2-image REQUIRED)
    if(${VCPKG_TOOLCHAIN})
		set(LIBRARIES
			SDL2::SDL2 SDL2::SDL2main
			SDL2::SDL2_image)

        add_compile_definitions(VCPKG_TOOLCHAIN)
    else()
		set(LIBRARIES
			SDL2::SDL2 SDL2::SDL2main
			SDL2::SDL2_image)
    endif()
# TODO: Other platforms; macOS for sure. Though pkg-config will work on macOS on command line with brew.
endif()

#configure_file(include/Config.hpp.in Config.hpp @ONLY)

set(SOURCES
    BMFontTest.cpp
    tinyxml2.cpp)

set(HEADERS
    PDBMFont.hpp
    tinyxml2.h)
source_group("Header Files" FILES ${HEADERS})

add_executable(BMFontTest ${SOURCES} ${HEADERS})

if(${PKG_CONFIG_FOUND})
    target_compile_options(BMFontTest PUBLIC
        ${SDL2_CFLAGS}
        ${SDL2_image_CFLAGS}

        ${SDL2_CFLAGS_OTHER}
        ${SDL2_image_CFLAGS_OTHER})
    target_link_directories(BMFontTest PUBLIC
        ${SDL2_LIBRARY_DIRS}
        ${SDL2_image_LIBRARY_DIRS})
    set(INCLUDE_DIRS
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_image_INCLUDE_DIRS})
endif()

#target_include_directories(BMFontTest PUBLIC ${PROJECT_BINARY_DIR} PRIVATE ${INCLUDE_DIRS})

target_link_libraries(BMFontTest PRIVATE ${LIBRARIES})
