cmake_minimum_required (VERSION 3.1)

project(game)

add_executable(game
	asteroid.cpp
	asteroidsManager.h
	mainGame.cpp
	movement.cpp
	asteroid.h
	game.cpp
	missilesManager.cpp
	movement.h
	asteroidsManager.cpp
	game.h
	missilesManager.h
)

target_link_libraries(game engine utils)

install(TARGETS game DESTINATION bin)