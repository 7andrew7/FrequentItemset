cmake_minimum_required (VERSION 2.8)
project (apriori)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -g -std=c++11")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O3 -g -DNDEBUG -std=c++11")
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(apriori apriori.cpp)

ADD_SUBDIRECTORY (gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp files
add_executable(runUnitTests apriori_test.cpp )

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)
add_test( runUnitTests runUnitTests )
