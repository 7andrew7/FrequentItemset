cmake_minimum_required (VERSION 2.8)
project (apriori)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -g -std=c++11")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -O3 -g -DNDEBUG -std=c++11")
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(apriori apriori.cpp)

