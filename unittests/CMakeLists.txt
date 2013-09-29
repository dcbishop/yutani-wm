################################################################################
# dglw unit test CMake file. - The CMake script for building the project.      #
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
string(TOLOWER ${PROJECT_NAME}_unittest PROJECT_TEST_NAME)

# This uses the official FindGTest cmake, but it doesn't make much sense 
# as the libraries have to be manually built for the specific compiler/arch.
#find_package(GTest)
#if(!GTEST_FOUND)
#   message(AUTHOR_WARNING "Could not find systemwide Google Test libraries...")
#   #return()
#else()
#   list(APPEND COMMON_LIBS "${GTEST_BOTH_LIBRARIES}")
#endif()

# Hack to build systemwide gtest, might fail on non-Ubuntu.
# [TODO]: Look at including it in extern.
#if(!GTEST_FOUND)
   message("Using systemwide GTest")
   add_subdirectory( /usr/src/gtest ${PROJECT_BINARY_DIR}/gtest )
   include_directories( "/usr/include/gtest" )
   list(APPEND COMMON_LIBS "gtest;gtest_main;pthread")
#endif()

# Register the package with CMakes built in testing system ctest
enable_testing()
list(REMOVE_ITEM SOURCES "${MAIN_SOURCE}")
file(GLOB_RECURSE TEST_SOURCES "${TESTS_DIRECTORY}/*.cpp")
add_executable(${PROJECT_TEST_NAME} ${TEST_SOURCES} ${SOURCES})
target_link_libraries(${PROJECT_TEST_NAME} ${COMMON_LIBS})
add_test(${PROJECT_TEST_NAME}, ${PROJECT_TEST_NAME})
if(HAVE_QT)
   qt5_use_modules(${PROJECT_TEST_NAME} Core Widgets Gui OpenGL)
   add_definitions(-DUSE_QT)
endif()
