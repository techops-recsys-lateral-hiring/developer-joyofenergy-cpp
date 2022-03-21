add_executable(endpoint_test)
target_sources(endpoint_test PRIVATE ReadingTest.cpp)
find_package(GTest REQUIRED)
find_package(Boost REQUIRED)
# Turn off magic autolink esp for Windows
target_compile_definitions(Boost::headers INTERFACE BOOST_ALL_NO_LIB)
find_package(nlohmann_json REQUIRED)
target_link_libraries(endpoint_test PRIVATE GTest::gmock_main Boost::headers nlohmann_json::nlohmann_json rest)
add_test(endpoint_test endpoint_test)

add_subdirectory(domain)
add_subdirectory(service)
add_subdirectory(controller)