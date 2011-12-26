cmake_minimum_required(VERSION 2.8)

project( program )
file(GLOB program_SRCS src/*/*cpp )

# Get the git repo info
exec_program(
	"git"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS "describe --always"
	OUTPUT_VARIABLE GIT_VERSION )

exec_program(
        "git"
        ${CMAKE_CURRENT_SOURCE_DIR}
        ARGS "rev-parse --verify HEAD"
        OUTPUT_VARIABLE GIT_SHA1 )

exec_program(
	"date"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS "+\"%Y-%m-%d %H:%M\""
	OUTPUT_VARIABLE BUILD_DATE )

# Check to see we got valid git versions
if( "${GIT_SHA1}" STREQUAL "" )
set( GIT_SHA1 "\"UNKNOWN\"" )
endif( "${GIT_SHA1}" STREQUAL "" )

if( "${GIT_VERSION}" STREQUAL "" )
set( GIT_VERSION "\"UNKNOWN\"" )
endif( "${GIT_VERSION}" STREQUAL "" )

if( "${BUILD_DATE}" STREQUAL "" )
set( BUILD_DATE "\"UNKNOWN\"" )
endif( "${BUILD_DATE}" STREQUAL "" )

add_definitions("-DGIT_SHA1=\"${GIT_SHA1}\"")
add_definitions("-DGIT_VERSION=\"${GIT_VERSION}\"")
add_definitions("-DBUILD_DATE=\"${BUILD_DATE}\"")

add_executable( program ${program_SRCS} )
