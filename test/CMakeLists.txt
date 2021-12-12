add_definitions(-DBOOST_ALL_NO_LIB)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601)
endif()

add_executable(endpoint_test)
target_sources(endpoint_test PRIVATE ReadingTest.cpp)
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(endpoint_test PRIVATE GTest::gmock_main nlohmann_json::nlohmann_json rest)

add_executable(controller_test)
target_sources(controller_test PRIVATE controller/MeterReadingControllerTest.cpp controller/PricePlanComparatorControllerTest.cpp)
target_link_libraries(controller_test PRIVATE GTest::gmock_main nlohmann_json::nlohmann_json rest)

add_executable(domain_test)
target_sources(domain_test PRIVATE domain/PricePlanTest.cpp)
target_link_libraries(domain_test PRIVATE GTest::gmock_main rest)

add_executable(service_test)
target_sources(service_test PRIVATE service/MeterReadingServiceTest.cpp service/PricePlanServiceTest.cpp)
target_link_libraries(service_test PRIVATE GTest::gmock_main rest)

add_test(endpoint_test endpoint_test)
add_test(controller_test controller_test)
add_test(domain_test domain_test)
add_test(service_test service_test)
