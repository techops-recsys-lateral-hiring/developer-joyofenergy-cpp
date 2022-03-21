add_executable(service_test)
target_sources(service_test PRIVATE MeterReadingServiceTest.cpp)
target_link_libraries(service_test PRIVATE GTest::gmock_main service)
add_test(service_test service_test)
