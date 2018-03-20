cmake_minimum_required (VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project (utils)

SET(SOURCE
  	private/absolutePath.cpp
	private/debug.cpp
	private/fileStream.cpp
	private/fileUtils.cpp
	private/image.cpp
	private/random.cpp
	private/timer.cpp
	private/types.cpp
)

SET(HEADERS
	public/absolutePath.h
	public/debug.h
	public/fileStream.h
	public/fileUtils.h
	public/hash.h
	public/image.h
	public/macros.h
	public/matrix.h
	public/random.h
	public/stream.h
	public/timer.h
	public/types.h
	public/vector2.h
	public/vector4.h
	public/wndincl.h
)

add_library(utils STATIC ${SOURCE} ${HEADERS})

target_include_directories(utils PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
)

install(TARGETS utils DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)