add_executable(endpoint_test)
target_sources(endpoint_test PRIVATE readings.cpp)
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(endpoint_test PRIVATE GTest::gmock_main nlohmann_json::nlohmann_json rest)

add_test(endpoint_test endpoint_test)