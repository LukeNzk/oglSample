cmake_minimum_required (VERSION 3.1)

project(engine)

set(PRIVATE
	private/engine.cpp
	private/glExtenstions.cpp
	private/glExtenstions.h
	private/graphics.cpp
	private/graphics.h
	private/renderer.cpp
	private/renderer.h
	private/sprite.cpp
	private/spritesRenderer.cpp
	private/spritesRenderer.h
	private/textureManager.cpp
	private/window.cpp
	private/window.h
)

set(PUBLIC
	public/engine.h
	public/game.h
	public/input.h
	public/inputCodes.h
	public/sprite.h
	public/textureManager.h
)

add_library(engine ${PRIVATE} ${PUBLIC})

target_link_libraries(engine utils)

target_include_directories(engine PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>)

install(TARGETS engine DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)