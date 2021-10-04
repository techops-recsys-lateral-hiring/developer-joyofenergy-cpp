add_executable(endpoint_test)
target_sources(endpoint_test PRIVATE ReadingTest.cpp)
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(endpoint_test PRIVATE GTest::gmock_main nlohmann_json::nlohmann_json rest)

add_executable(controller_test)
target_sources(controller_test PRIVATE controller/MeterReadingControllerTest.cpp controller/PricePlanComparatorControllerTest.cpp)
target_link_libraries(controller_test PRIVATE GTest::gmock_main nlohmann_json::nlohmann_json rest)

add_test(endpoint_test endpoint_test)
add_test(controller_test controller_test)