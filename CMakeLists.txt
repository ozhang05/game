cmake_minimum_required(VERSION 3.10)

project(Game)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fsanitize=address -g")
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile commands" FORCE)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fsanitize=address -g -O2")

# List the include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/src/engine/battle_system
    ${CMAKE_SOURCE_DIR}/src/engine/core
    ${CMAKE_SOURCE_DIR}/src/engine/editor
    ${CMAKE_SOURCE_DIR}/src/engine/entity_system
    ${CMAKE_SOURCE_DIR}/src/engine/game
    ${CMAKE_SOURCE_DIR}/src/engine/input_system
    ${CMAKE_SOURCE_DIR}/src/engine/map_code
    ${CMAKE_SOURCE_DIR}/src/engine/menu_system
    ${CMAKE_SOURCE_DIR}/src/engine/render_system
    ${CMAKE_SOURCE_DIR}/src/engine/physics_system
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/test_files
)

# Define specific source file groups for different packages

file(GLOB BATTLE_FILES
    src/engine/battle_system/*.cpp
)

file(GLOB ENGINE_FILES
    src/engine/core/*.cpp
)

file(GLOB MAP_MAKER_FILES
    src/engine/editor/*.cpp
)

file(GLOB ENTITY_FILES
    src/engine/entity_system/*.cpp
)

file (GLOB GAME_FILES
    src/engine/game/*.cpp
)

file (GLOB INPUT_FILES
    src/engine/input_system/*.cpp
)

file(GLOB MAP_FILES
    src/engine/map_code/*.cpp
)

file (GLOB MENU_FILES
    src/engine/menu_system/*.cpp
)

file(GLOB RENDER_FILES
    src/engine/render_system/*.cpp
)

file(GLOB PHYSICS_FILES
    src/engine/physics_system/*.cpp
)

add_library(core STATIC
    ${BATTLE_FILES}
    ${ENGINE_FILES}
    ${ENTITY_FILES}
    ${INPUT_FILES}
    ${MAP_FILES}
    ${MENU_FILES}
    ${RENDER_FILES}
    ${PHYSICS_FILES}
)

################################################################################


find_package(SFML COMPONENTS System Window Graphics Audio CONFIG REQUIRED)
set(SFML_LIBS SFML::System SFML::Window SFML::Graphics SFML::Audio)
find_package(Lua 5.4.0 REQUIRED)
find_package(nlohmann_json 3.11.0 REQUIRED)

target_link_libraries(core PRIVATE
    ${SFML_LIBS}
    ${LUA_LIBRARIES}
    nlohmann_json::nlohmann_json
)
target_include_directories(core PRIVATE
    ${SFML_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include/
)

# Compile all source files into an object
add_executable(game
    src/main.cpp
    src/engine/editor/EntityMenu.cpp
    src/engine/editor/EntityMenu.h
    ${GAME_FILES}
)

add_executable(map_maker
    ${MAP_MAKER_FILES}
)

# game specific links
target_link_libraries(game PRIVATE
    ${SFML_LIBS}
    ${LUA_LIBRARIES}
)
target_include_directories(game PRIVATE
    ${SFML_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
)

# map maker specific links
target_link_libraries(map_maker PRIVATE
    ${SFML_LIBS}
    ${LUA_LIBRARIES}
)
target_include_directories(map_maker PRIVATE
    ${SFML_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
)

target_link_libraries(game PRIVATE core)
target_link_libraries(map_maker PRIVATE core)

find_program(IWYU_EXECUTABLE NAMES include-what-you-use)
if (IWYU_EXECUTABLE)
    add_custom_target(iwyu
        COMMAND ${CMAKE_COMMAND} -DSET_IWYU=ON ${CMAKE_SOURCE_DIR}
        COMMENT "Configuring IWYU for game"
        COMMAND make game
    )
    if (DEFINED SET_IWYU AND SET_IWYU)
        set_target_properties(game PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${IWYU_EXECUTABLE})
    else()
        set_target_properties(game PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")
    endif()
else()
    message(STATUS "IWYU not found. Please install IWYU for automatic checking.")
endif()
