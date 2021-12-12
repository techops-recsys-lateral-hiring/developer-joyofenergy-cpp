#include <configuration.h>
#include <date/date.h>
#include <gmock/gmock.h>
#include <rest/service/PricePlanService.h>

#include <sstream>

struct PricePlanServiceTest : public ::testing::Test {
  PricePlanServiceTest() : meterAssociatedReadings_(setupReadings()), price_plans_(setupPricePlans()) {}

  std::unordered_map<std::string, std::vector<ElectricityReading>> meterAssociatedReadings_;
  MeterReadingService meterReadingService_{meterAssociatedReadings_};

  std::vector<PricePlan> price_plans_;
  PricePlanService pricePlanService_{price_plans_, meterReadingService_};

  static std::unordered_map<std::string, std::vector<ElectricityReading>> setupReadings() {
    return {
            {
              "meter-0", {
                createElectricityReading("2021-08-18T06:42:15.725202Z", 20),
                createElectricityReading("2021-08-20T06:42:15.725202Z", 30)
              }
            },
            {
              "meter-1", {
                 createElectricityReading("2021-12-10T06:42:15Z", 30),
                 createElectricityReading("2021-12-09T06:42:15Z", 40),
              }
            }
           };
  }

  static std::vector<PricePlan> setupPricePlans() { return {PricePlan("plan-0", "company-0", 3, {})}; }

  using time_point_type = ElectricityReading::time_point_type;

  static time_point_type parseRfc3339(std::string const &rfc3339) {
    time_point_type tp;
    std::istringstream ss(rfc3339);
    ss >> date::parse("%FT%TZ", tp);
    return tp;
  }

  static ElectricityReading createElectricityReading(std::string const &rfc3339, std::size_t reading) {
    return ElectricityReading(parseRfc3339(rfc3339), reading);
  }
};

TEST_F(PricePlanServiceTest, GetConsumptionCostShouldReturnNullGivenMeterIdThatDoesNotExist) {
  EXPECT_FALSE(pricePlanService_.getConsumptionCostOfElectricityReadingsForEachPricePlan("unknown-id"));
}

TEST_F(PricePlanServiceTest, GetConsumptionCostShouldReturnValidValuesForEachPlanGivenMeterIdExists) {
  auto const &result = pricePlanService_.getConsumptionCostOfElectricityReadingsForEachPricePlan("meter-1");
  EXPECT_TRUE(result.has_value());

  auto cost = result->at("plan-0");
  EXPECT_EQ(cost, 3);
}
