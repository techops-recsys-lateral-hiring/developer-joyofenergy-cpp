#include <gmock/gmock.h>
#include <rest/domain/PricePlan.h>
#include <rest/controller/MeterReadingController.h>

using ::testing::Eq;

TEST(PricePlanTest, GetEnergySupplierShouldReturnTheEnergySupplierGivenInTheConstructor) {
  const std::string energy_supplier = "Energy Supplier Name";

  PricePlan p("", energy_supplier, 0, {});

  EXPECT_THAT(p.getEnergySupplier(), energy_supplier);
}

TEST(PricePlanTest, GetPriceShouldReturnTheBasePriceGivenAnOrdinaryDateTime) {
  auto time = detail::fromRfc3339("2021-09-30T06:42:15.725202Z");
  PricePlan::PeakTimeMultiplier peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::WEDNESDAY, 10);
  PricePlan price_plan("", "", 1, {peak_time_multiplier});

  auto price = price_plan.getPrice(time);

  EXPECT_THAT(price, 1);
}

TEST(PricePlanTest, GetPriceShouldReturnAnExceptionPriceGivenExceptionalDateTime) {
  auto time = detail::fromRfc3339("2021-09-29T06:42:15.725202Z");
  PricePlan::PeakTimeMultiplier peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::WEDNESDAY, 10);
  PricePlan price_plan("", "", 1, {peak_time_multiplier});

  auto price = price_plan.getPrice(time);

  EXPECT_THAT(price, 10);
}

TEST(PricePlanTest, GetPriceShouldReceiveMultipleExceptionalDateTimes) {
  auto time = detail::fromRfc3339("2021-09-29T06:42:15.725202Z");
  PricePlan::PeakTimeMultiplier peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::WEDNESDAY, 10);
  PricePlan::PeakTimeMultiplier another_peak_time_multiplier(PricePlan::PeakTimeMultiplier::DayOfWeek::THURSDAY, 10);
  PricePlan price_plan("", "", 1, {peak_time_multiplier, another_peak_time_multiplier});

  auto price = price_plan.getPrice(time);

  EXPECT_THAT(price, 10);
}
