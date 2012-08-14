cmake_minimum_required(VERSION 2.6)

project(unnamed)

# Projects default options
option( C++11        "C++11"     ON  )
option( Profile      "Profile"   OFF )
option( Boost        "Boost"     OFF )
option( Debug        "Debug"     OFF )

# Add custom debug build compiler flags here...
#list(APPEND CMAKE_CXX_FLAGS_DEBUG "-O3")

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

# Note this is the 'bad' way of doing this, but the recommended alternative is
# to keep a list of *all* source files =/
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

# Set the defines
# Build time define
include(GetDateTime)
DATETIME(BUILD_DATETIME)
add_definitions(-DBUILD_DATE_="${BUILD_DATETIME}")

# Build type
if(CMAKE_BUILD_TYPE)
   add_definitions(-DBUILD_TYPE_="${CMAKE_BUILD_TYPE}")
   message("Build type: ${CMAKE_BUILD_TYPE}")
else()
   add_definitions(-DBUILD_TYPE_="Default")
   message("Default build type...")
endif()

# Git version info
include(GetGitRevisionDescription)
git_describe(GIT_DESCRIBE "--always")
add_definitions(-DGIT_VERSION_="${GIT_DESCRIBE}")

# Git SHA1 define
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
add_definitions(-DGIT_SHA1_="${GIT_SHA1}")

# Detect clang++ compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   set(CMAKE_COMPILER_IS_CLANGXX_)
endif()

# Compiler specific flags...
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX_)
   if(C++11)
      message("Enabling C++11 support...")
      list(APPEND CMAKE_CXX_FLAGS "-std=c++0x")
      add_definitions(-DUSE_CPP11_)
   endif()

   list(APPEND CMAKE_CXX_FLAGS_RELEASE "-O3 -Werror") #-Weffc++
   
   if(Profile)
      message("Profiling: ON...")
      list(APPEND CMAKE_CXX_FLAGS "-pg")
   endif()
endif()

# The main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add Boost libs
if(Boost)
   find_package(Boost COMPONENTS filesystem system iostreams REQUIRED)
   if(Boost_FOUND)
      include_directories(${Boost_INCLUDE_DIRS})
      add_definitions(-DUSE_BOOST_)
      target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
   endif()
endif()

