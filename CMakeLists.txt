cmake_minimum_required(VERSION 3.16)
project(master_rpc)


set(CMAKE_CXX_STANDARD 20)
# include (cmake/util.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(.)
include_directories(/usr/include)
link_directories(/usr/lib)

set(LIB_SRC
        gen-cpp/Master.cpp
        gen-cpp/master_constants.cpp
        gen-cpp/master_types.cpp
        gen-cpp/Slave.cpp
        gen-cpp/slave_constants.cpp
        gen-cpp/slave_types.cpp
        db/mysql_rsync.cpp
		db/mysql_client.cpp
        conf/conf.cpp
        conf/dbconf.cpp
		thread/thread.cpp
		thread/mutex.cpp
		thread/threadpool.cpp
		log/elog.cpp
		log/elog_async.cpp
		log/elog_buf.cpp
		log/elog_file.cpp
		log/elog_port.cpp
		log/elog_utils.cpp
        )

add_library(rpc SHARED ${LIB_SRC})

set(LIB_LIB
        rpc
        pthread
        thrift
        thriftnb
        event
		mysqlclient
        )

add_executable(master_server gen-cpp/Master_server.cpp)
add_dependencies(master_server rpc)
target_link_libraries(master_server rpc ${LIB_LIB})

add_executable(master_client client/client.cpp)
add_dependencies(master_client rpc)
target_link_libraries(master_client rpc ${LIB_LIB})

add_executable(thread_test thread/thread_test.cpp)
add_dependencies(thread_test rpc)
target_link_libraries(thread_test rpc ${LIB_LIB})
