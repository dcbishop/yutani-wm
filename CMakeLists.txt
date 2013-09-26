################################################################################
# CMake file. - The CMake script for building the project.                     #
################################################################################
# Written in 2012 by David C. Bishop david@davidbishop.org                     #
#                                                                              #
# To the extent possible under law, the author(s) have dedicated all copyright #
# and related and neighboring rights to this software to the public domain     #
# worldwide. This software is distributed without any warranty.                #
#                                                                              #
# You should have received a copy of the CC0 Public Domain Dedication along    #
# with this software. If not, see                                              #
# <http://creativecommons.org/publicdomain/zero/1.0/>.                         #
################################################################################
# NOTE: The CC0 license applies only to this cmake file, not the entire        #
# project (unless otherwise stated). I just like ensuring there is no problem  #
# with people needing snippits of code for their CMake stuff.                  #
################################################################################
cmake_minimum_required(VERSION 2.6)

project(Unnamed)
message("Project ${PROJECT_NAME}...")


##############################
#  Projects default options  #
##############################
option( C++11        "C++11"     ON  )
option( Profile      "Profile"   OFF )
option( Boost        "Boost"     OFF )
option( Debug        "Debug"     OFF )
option( Testing      "Testing"   ON  )
option( Coverage     "Coverage"  OFF )


###########################
#  Some useful variables  #
###########################
set(SOURCES_SUBDIR src)
set(HEADERS_SUBDIR src)
set(TESTS_SUBDIR tests)
set(SOURCES_DIRECTORY ${CMAKE_SOURCE_DIR}/${SOURCES_SUBDIR})
set(HEADERS_DIRECTORY ${CMAKE_SOURCE_DIR}/${HEADERS_SUBDIR})
set(TESTS_DIRECTORY ${CMAKE_SOURCE_DIR}/${TESTS_SUBDIR})
set(CONFILE_FILENAME "Globals.hpp")
set(CONFILE_SUBDIR "${HEADERS_SUBDIR}/Main")
set(CONFILE_FILE "${PROJECT_SOURCE_DIR}/${CONFILE_SUBDIR}/${CONFILE_FILENAME}.in")
string(TOLOWER ${PROJECT_NAME} LOWER_NAME)
string(TOUPPER ${PROJECT_NAME} UPPER_NAME)
set(BIN_NAME ${LOWER_NAME})

# Add custom debug build compiler flags here...
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")

# Detect clang++ compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   set(CMAKE_COMPILER_IS_CLANGXX_ TRUE)
endif()

# Generate compile_commands.json, useful for tools...
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#############################
#  Set install directories  #
#############################
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for dynamic libraries")
set(INSTALL_ARCHIVE_DIR lib CACHE PATH "Installation directory for static libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")


###############
#  Debugging  #
###############
# Ensure that setting the debug option also sets the build type and visaversa
if(Debug)
   set(CMAKE_BUILD_TYPE "Debug")
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
   option(Debug "Debug" ON)
endif()

# Set _DEBUG #define for project
if(Debug)
   add_definitions(-D_DEBUG)
endif()


#############################
#  Find the stuff to build  #
#############################
# Note this is the 'bad' way of doing this, but the recommended 
# alternative is to manually keep a list of *all* source files =/
file(GLOB_RECURSE SOURCES "${SOURCES_DIRECTORY}/*.cpp")
file(GLOB_RECURSE HEADERS "${HEADERS_DIRECTORY}/*.h*")
file(GLOB_RECURSE MAIN_SOURCE "${SOURCES_DIRECTORY}/Main.cpp")

include_directories("${HEADERS_DIRECTORY}/")

# CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


###################################
#  Pass variables to the program  #
###################################

# Get build time
include(GetDateTime)
DATETIME(BUILD_DATETIME)

# Build type
if(CMAKE_BUILD_TYPE)
   message("Build type: ${CMAKE_BUILD_TYPE}")
else()
   message(STATUS "Using default build type...")
endif()

# Git version info
include(GetGitRevisionDescription)
git_describe(GIT_DESCRIBE "--always")

# Get Git SHA1 hash
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# Compiler specific flags...
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX_)
   if(C++11)
      message(STATUS "Enabling C++11 support...")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
      add_definitions(-DUSE_CPP11_)
      add_definitions(-DGTEST_LANG_CXX11)

      # Use clangs library
      if(CMAKE_COMPILER_IS_CLANGXX_)
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
         list(APPEND COMMON_LIBS "-lc++abi")
      endif()
   endif()

   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Werror") #-Weffc++
   
   if(Profile)
      message(STATUS "Profiling: ON...")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
   endif()

   if(Coverage)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0")
      link_libraries(gcov)
   endif()
endif()

# Genearte config header file
configure_file("${CONFILE_FILE}" "${PROJECT_BINARY_DIR}/${CONFILE_SUBDIR}/${CONFILE_FILENAME}" @ONLY)
include_directories(${PROJECT_BINARY_DIR}/${CONFILE_SUBDIR})


######################
#  Handle libraries  #
######################

# Add Boost libs
if(Boost)
   find_package(Boost COMPONENTS filesystem system iostreams REQUIRED)
   if(Boost_FOUND)
      include_directories(${Boost_INCLUDE_DIRS})
      add_definitions(-DUSE_BOOST_)
      list(APPEND COMMON_LIBS ${Boost_LIBRARIES})
   endif()
endif()


#######################
#  Build the project  #
#######################
# The main executable
add_executable(${BIN_NAME} ${SOURCES})
target_link_libraries(${BIN_NAME} ${COMMON_LIBS})


#############
#  Install  #
#############
install(FILES ${PROJECT_BINARY_DIR}/${BIN_NAME} DESTINATION bin)


#############
#  Testing  #
#############
if(Testing)
   add_subdirectory(${TESTS_SUBDIR})
endif()
