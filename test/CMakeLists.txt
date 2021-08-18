add_executable(container_test)
target_sources(container_test PRIVATE readings.cpp)
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(container_test PRIVATE GTest::gmock_main nlohmann_json::nlohmann_json rest)

add_test(container_test container_test)