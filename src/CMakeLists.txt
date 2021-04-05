cmake_minimum_required( VERSION 3.16.3 )
project( Raycaster VERSION 0.1 LANGUAGES CXX )

set( SFML_CMAKE C:/Users/bugra/Desktop/Work/Blackboard/C++/LIBS/SFML-2.5.1/lib/cmake )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_EXTENSIONS OFF )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build (Debug or Release)" FORCE )
endif()

set ( SOURCE_TILEMAP ./TileMap/TileMap.cpp )
add_library( TILEMAP SHARED ${SOURCE_TILEMAP} )

set ( SOURCE_ENTITIES ./Entities/Entities.cpp )
add_library( ENTITIES SHARED ${SOURCE_ENTITIES} )

set ( SOURCE_UI ./UI/UI.cpp )
add_library( UI SHARED ${SOURCE_UI} )

set( SOURCE_EXE ./main.cpp )
add_executable( ${PROJECT_NAME} ${SOURCE_EXE} )

list( APPEND CMAKE_PREFIX_PATH ${SFML_CMAKE} )
find_package( SFML 2.5 COMPONENTS audio network graphics window system REQUIRED )
target_link_libraries( TILEMAP PUBLIC sfml-graphics )
target_link_libraries( ENTITIES PUBLIC sfml-graphics PRIVATE TILEMAP)
target_link_libraries( UI PUBLIC sfml-graphics )
target_link_libraries( ${PROJECT_NAME} PRIVATE TILEMAP ENTITIES UI PUBLIC sfml-system)