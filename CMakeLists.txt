cmake_minimum_required(VERSION 2.6)
project(Kumario)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Kumario_SOURCE_DIR}/cmake")
set(BIN_DIR ${Kumario_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc and build in debug mode
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_Mixer REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})

file(COPY Kumar/assets DESTINATION .)

include_directories(Kumar)

file(GLOB_RECURSE Kumario_SOURCES "Kumar/*.cpp")
add_executable (Kumario ${Kumario_SOURCES})

target_link_libraries(Kumario ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY})

install(TARGETS Kumario RUNTIME DESTINATION ${BIN_DIR})
