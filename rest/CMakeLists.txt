add_subdirectory(service)
add_subdirectory(controller)
add_library(rest)
target_sources(rest PRIVATE server.cpp)
target_include_directories(rest INTERFACE ..)

target_compile_definitions(rest PRIVATE BOOST_ALL_NO_LIB)

if(WIN32)
  target_compile_definitions(rest PRIVATE _WIN32_WINNT=0x0601)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(rest PRIVATE Threads::Threads)

target_link_libraries(rest PUBLIC controller)

find_package(date REQUIRED)
target_link_libraries(rest PUBLIC date::date)
