#include <domain/PricePlan.h>
#include <gmock/gmock.h>

using ::testing::Eq;

namespace {
auto FromRfc3339(const std::string &time) {
  std::tm tm = {};
  std::stringstream ss(time);
  ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
  auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  return tp;
}
}  // namespace

TEST(PricePlanTest, GetEnergySupplierShouldReturnTheEnergySupplierGivenInTheConstructor) {
  const std::string energy_supplier = "Energy Supplier Name";

  PricePlan p("", energy_supplier, 0, {});

  EXPECT_THAT(p.getEnergySupplier(), energy_supplier);
}

TEST(PricePlanTest, GetPriceShouldReturnTheBasePriceGivenAnOrdinaryDateTime) {
  auto time = FromRfc3339("2021-09-30T06:42:15.725202Z");
  PricePlan::PeakTimeMultiplier peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::WEDNESDAY, 10);
  PricePlan price_plan("", "", 1, {peak_time_multiplier});

  auto price = price_plan.getPrice(time);

  EXPECT_THAT(price, 1);
}

TEST(PricePlanTest, GetPriceShouldReturnAnExceptionPriceGivenExceptionalDateTime) {
  auto time = FromRfc3339("2021-09-29T06:42:15.725202Z");
  PricePlan::PeakTimeMultiplier peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::WEDNESDAY, 10);
  PricePlan price_plan("", "", 1, {peak_time_multiplier});

  auto price = price_plan.getPrice(time);

  EXPECT_THAT(price, 10);
}

TEST(PricePlanTest, GetPriceShouldReceiveMultipleExceptionalDateTimes) {
  auto time = FromRfc3339("2021-09-29T06:42:15.725202Z");
  PricePlan::PeakTimeMultiplier peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::WEDNESDAY, 10);
  PricePlan::PeakTimeMultiplier another_peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::THURSDAY, 10);
  PricePlan price_plan("", "", 1, {peak_time_multiplier, another_peak_time_multiplier});

  auto price = price_plan.getPrice(time);

  EXPECT_THAT(price, 10);
}
