add_executable(domain_test)
target_sources(domain_test PRIVATE PricePlanTest.cpp)
target_link_libraries(domain_test PRIVATE GTest::gmock_main domain)
add_test(NAME domain_test COMMAND domain_test)
