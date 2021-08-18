#include <nlohmann/json.hpp>

#include "EndpointTest.h"

using nlohmann::json;
using ::testing::Eq;
using ::testing::SizeIs;

class ReadingsTest : public EndpointTest {
};

TEST_F(ReadingsTest, ReadReadingShouldRespondWithAllReadingsGivenValidMeter) {
    auto res = client.Get("/readings/read/smart-meter-1");

    ASSERT_THAT(res.result(), Eq(http::status::ok));
    auto body = json::parse(res.body());
    auto readings = body["readings"];
    EXPECT_THAT(readings, SizeIs(20));
}

TEST_F(ReadingsTest, ReadReadingShouldRespondNotFoundGivenInvalidMeter) {
    auto res = client.Get("/readings/read/smart-meter-not-existed");

    ASSERT_THAT(res.result(), Eq(http::status::not_found));
}