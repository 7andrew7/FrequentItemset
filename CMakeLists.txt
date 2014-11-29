cmake_minimum_required (VERSION 2.8)
project (apriori)

find_package(Boost 1.54.0 REQUIRED timer system)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -g -std=c++11")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O3 -g -DNDEBUG -std=c++11")
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

ADD_SUBDIRECTORY (gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(mushroom.exe mushroom.cpp)
target_link_libraries(mushroom.exe ${Boost_LIBRARIES})

################################
# Unit Tests
################################
# Add test cpp files
add_executable(apriori_test.exe apriori_test.cpp)
target_link_libraries(apriori_test.exe ${Boost_LIBRARIES})
target_link_libraries(apriori_test.exe gtest gtest_main)
add_test( apriori_test apriori_test.exe)

add_executable(basket_set_test.exe basket_set_test.cpp)
target_link_libraries(basket_set_test.exe gtest gtest_main)
add_test( basket_set_test basket_set_test.exe )
