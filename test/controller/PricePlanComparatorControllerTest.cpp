#include <gmock/gmock.h>
#include <rest/configuration.h>
#include <rest/controller/PricePlanComparatorController.h>

#include <string>

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

using nlohmann::json;
using ::testing::Eq;

class PricePlanComparatorControllerTest : public ::testing::Test {
 protected:
  std::unordered_map<std::string, std::vector<ElectricityReading>> meterAssociatedReadings;
  MeterReadingService meterReadingService{meterAssociatedReadings};
  std::vector<PricePlan> price_plans = pricePlans();
  PricePlanService pricePlanService{price_plans, meterReadingService};
  PricePlanComparatorController controller{pricePlanService};
};

TEST_F(PricePlanComparatorControllerTest, CompareShouldCalculateCostForMeterReadingsForEveryPricePlan) {
  const std::string smart_meter_id = "smart-meter-0";
  std::vector<ElectricityReading> readings = {{std::chrono::system_clock::now() - std::chrono::hours{1}, 15 * 10000},
                                              {std::chrono::system_clock::now(), 5 * 10000}};
  meterReadingService.storeReadings(smart_meter_id, readings);
  http::request<http::string_body> req;
  std::vector<std::string> queries = {smart_meter_id};

  auto response = controller.Compare(req, queries);

  auto body = json::parse(response.body());
  EXPECT_THAT(body["pricePlanId"], Eq("price-plan-0"));
  EXPECT_THAT(body["pricePlanComparisons"]["price-plan-0"], Eq(100));
  EXPECT_THAT(body["pricePlanComparisons"]["price-plan-1"], Eq(20));
  EXPECT_THAT(body["pricePlanComparisons"]["price-plan-2"], Eq(10));
}

TEST_F(PricePlanComparatorControllerTest, RecommandShouldRecommandCheapestPricePlansNoLimitForMeterUsage) {
  const std::string smart_meter_id = "smart-meter-0";
  std::vector<ElectricityReading> readings = {
      {std::chrono::system_clock::now() - std::chrono::seconds(1800), 35 * 10000},
      {std::chrono::system_clock::now(), 3 * 10000}
  };
  meterReadingService.storeReadings(smart_meter_id, readings);
  http::request<http::string_body> req;
  std::vector<std::string> queries = {smart_meter_id};

  auto response = controller.Recommend(req, queries);

  auto body = json::parse(response.body());
  auto recommend = body["recommend"];
  EXPECT_THAT(recommend.size(), Eq(3));
  EXPECT_THAT(body["recommend"][0]["price-plan-2"], Eq(38));
  EXPECT_THAT(body["recommend"][1]["price-plan-1"], Eq(76));
  EXPECT_THAT(body["recommend"][2]["price-plan-0"], Eq(380));
}

TEST_F(PricePlanComparatorControllerTest, RecommandShouldRecommendLimitedCheapestPricePlansForMeterUsage) {
  const std::string smart_meter_id = "smart-meter-0";
  std::vector<ElectricityReading> readings = {
      {std::chrono::system_clock::now() - std::chrono::seconds(2700), 5 * 10000},
      {std::chrono::system_clock::now(), 20 * 10000}
  };
  meterReadingService.storeReadings(smart_meter_id, readings);
  http::request<http::string_body> req;
  std::vector<std::string> queries = {smart_meter_id, "limit", "2"};

  auto response = controller.Recommend(req, queries);

  auto body = json::parse(response.body());
  auto recommend = body["recommend"];
  EXPECT_THAT(recommend.size(), Eq(2));
  EXPECT_THAT(body["recommend"][0]["price-plan-2"], Eq(16.6666));
  EXPECT_THAT(body["recommend"][1]["price-plan-1"], Eq(33.3332));
}

TEST_F(PricePlanComparatorControllerTest, RecommandShouldRecommendCheapestPricePlansMoreThanLimitAvailableForMeterUsage) {
  const std::string smart_meter_id = "smart-meter-0";
  std::vector<ElectricityReading> readings = {{std::chrono::system_clock::now() - std::chrono::hours(1), 25 * 10000},
                                              {std::chrono::system_clock::now(), 3 * 10000}};
  meterReadingService.storeReadings(smart_meter_id, readings);
  http::request<http::string_body> req;
  std::vector<std::string> queries = {smart_meter_id, "limit", "5"};

  auto response = controller.Recommend(req, queries);

  auto body = json::parse(response.body());
  auto recommend = body["recommend"];
  EXPECT_THAT(recommend.size(), Eq(3));
  EXPECT_THAT(body["recommend"][0]["price-plan-2"], Eq(14));
  EXPECT_THAT(body["recommend"][1]["price-plan-1"], Eq(28));
  EXPECT_THAT(body["recommend"][2]["price-plan-0"], Eq(140));
}
TEST_F(PricePlanComparatorControllerTest, CompareShouldReturnNotFoundGivenNoMatchingMeterId) {
  http::request<http::string_body> req;
  std::vector<std::string> queries = {"meter-id-not-exist"};

  auto response = controller.Compare(req, queries);

  EXPECT_THAT(response.result(), Eq(http::status::not_found));
}