cmake_minimum_required (VERSION 3.1)

project(game)
find_package(FreeImage)

add_executable(game
	src/asteroid.cpp
	src/asteroidsManager.h
	src/mainGame.cpp
	src/movement.cpp
	src/asteroid.h
	src/game.cpp
	src/missilesManager.cpp
	src/movement.h
	src/asteroidsManager.cpp
	src/game.h
	src/missilesManager.h
)

target_link_libraries(game engine utils)

install(TARGETS game DESTINATION bin)