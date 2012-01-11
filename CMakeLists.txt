cmake_minimum_required(VERSION 2.6)

project(program)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

option(Profile "Profile" OFF)

# Note this is the 'bad' way of doing this, but the recommended alternative is
# to keep a list of *all* source files =/
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Set the defines
# Build time define
include(GetDateTime)
DATETIME(BUILD_DATETIME)
add_definitions(-DBUILD_DATE_="${BUILD_DATETIME}")

# Git version info
include(GetGitRevisionDescription)
git_describe(GIT_DESCRIBE "--always")
add_definitions(-DGIT_VERSION_="${GIT_DESCRIBE}")

# Build type
if(CMAKE_BUILD_TYPE)
   add_definitions(-DBUILD_TYPE_="${CMAKE_BUILD_TYPE}")
   message("Build type: ${CMAKE_BUILD_TYPE}")
else(CMAKE_BUILD_TYPE)
   add_definitions(-DBUILD_TYPE_="Default")
   message("Default build type...")
endif(CMAKE_BUILD_TYPE)

# Git SHA1 define
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
add_definitions(-DGIT_SHA1_="${GIT_SHA1}")

# Add debug define
if(CMAKE_BUILD_TYPE MATCHES "Debug") 
   add_definitions(-D_DEBUG)
endif(CMAKE_BUILD_TYPE MATCHES "Debug") 

# Add custom debug build compiler flags here...
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")

# GCC specific flags...
if(CMAKE_COMPILER_IS_GNUCC)
   #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Werror") #-Weffc++
   
   if(Profile)
      message("Profiling: ON...")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg") 
   endif(Profile)
endif(CMAKE_COMPILER_IS_GNUCC)

add_executable( program ${SOURCES} )
