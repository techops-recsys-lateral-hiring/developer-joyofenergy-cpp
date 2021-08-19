#ifndef DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H
#define DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H

#include <domain/ElectricityReading.h>
#include <domain/PricePlan.h>
#include <service/MeterReadingService.h>
#include <vector>
#include <ctime>
#include <optional>
#include <map>
#include <string>
#include <configuration.h>

class PricePlanService {
public:
    using time_point_type = std::chrono::time_point<std::chrono::system_clock>;

    std::optional<std::map<std::string, float>> getConsumptionCostOfElectricityReadingsForEachPricePlan(std::string smartMeterId){
        std::optional<std::vector<ElectricityReading>> electricityReadings = meterReadingService.getReadings(smartMeterId);
        if (!electricityReadings.has_value()) {
            return {};
        }

        std::map<std::string, float> consumptionCostOfElectricityReadingsForEachPricePlan;
        for(auto pricePlan:pricePlans){
            consumptionCostOfElectricityReadingsForEachPricePlan.insert(std::make_pair(pricePlan.getPlanName(), calculateCost(electricityReadings.value(), pricePlan)));
        }
        return consumptionCostOfElectricityReadingsForEachPricePlan;
    }

    PricePlanService(std::vector<PricePlan> pricePlans, MeterReadingService meterReadingService) :
            pricePlans(pricePlans), meterReadingService(meterReadingService) {}

private:
    const std::vector<PricePlan> pricePlans;
    MeterReadingService meterReadingService;

    int calculateCost(std::vector<ElectricityReading> electricityReadings, PricePlan pricePlan) {
        float average = calculateAverageReading(electricityReadings);
        float timeElapsed = calculateTimeElapsed(electricityReadings);

        float averagedCost = round(average / timeElapsed);
        return averagedCost * pricePlan.getUnitRate();
    }

    float calculateAverageReading(std::vector<ElectricityReading> electricityReadings) {
        float summedReadings = 0;
        for (ElectricityReading electricityReading : electricityReadings) {
            summedReadings += electricityReading.getReading();
        }

        return round(summedReadings / electricityReadings.size());
    }

    float calculateTimeElapsed(std::vector<ElectricityReading> electricityReadings) {
        ElectricityReading first = *electricityReadings.begin();
        ElectricityReading last = *electricityReadings.begin();
        std::vector<ElectricityReading>::iterator it;
        for(it = electricityReadings.begin(); it != electricityReadings.end(); it++){
            if (it->getTime() < first.getTime()){
                first = *it;
            }
            if (it->getTime() > first.getTime()){
                last = *it;
            }
        }

        std::chrono::steady_clock::duration duration = std::chrono::duration_cast<std::chrono::hours>(
                last.getTime() - first.getTime());
        return duration.count();
    }
};

#endif  // DEVELOPER_JOYOFENERGY_CPP_BEAST_PRICEPLANSERVICE_H
