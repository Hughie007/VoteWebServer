<<<<<<< HEAD
file(GLOB LIBSCPP "*.cpp" "./TcpConnection/*.cpp" "./TimeManager/*.cpp" "./syncTools/*.cpp" "./Reactor/*.cpp" "./Pool/*.cpp" "./Log/*.cpp")
set(SRCS
    ${LIBSCPP}
)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -g -std=c++11 -pthread")
add_executable(myVoteServer ${SRCS})
target_link_libraries(${PROJECT_NAME}  libmysqlclient.so)
=======
file(GLOB LIBSCPP "*.cpp" "./TcpConnection/*.cpp" "./TimeManager/*.cpp" "./syncTools/*.cpp" "./Reactor/*.cpp" "./Pool/*.cpp" "./Log/*.cpp" "./DbSyn/*.cpp")
set(SRCS
    ${LIBSCPP}
)
SET(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -g -pthread")
add_executable(myVoteServer ${SRCS})
target_include_directories(myVoteServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}  libmysqlclient.so)
target_link_libraries(${PROJECT_NAME}  libhiredis.so)
>>>>>>> 6d98924 (first common)
