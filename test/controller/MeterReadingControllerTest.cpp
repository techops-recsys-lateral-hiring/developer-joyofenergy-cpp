#include <controller/MeterReadingController.h>
#include <gmock/gmock.h>

#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>

using nlohmann::json;
using ::testing::Eq;

namespace http = boost::beast::http;

class MeterReadingControllerTest : public ::testing::Test {
 protected:
  std::unordered_map<std::string, std::vector<ElectricityReading>> meterAssociatedReadings;
  ElectricityReadingService electricityReadingService{meterAssociatedReadings};
  MeterReadingService meterReadingService{meterAssociatedReadings};
  MeterReadingController controller{electricityReadingService, meterReadingService};

  http::request<http::string_body> BuildRequest(http::verb verb, boost::string_view target, const json &request_body) {
    http::request<http::string_body> req{verb, target, 11};
    req.set(http::field::content_type, "application/json");
    req.body() = request_body.dump();
    req.prepare_payload();
    return req;
  }
};

TEST_F(MeterReadingControllerTest, StoreShouldResponseWithErrorGivenNoMeterIdIsSupplied) {
  auto req = BuildRequest(http::verb::post, "/readings/store", R"({})"_json);
  std::vector<std::string> queries;

  auto response = controller.Store(req, queries);

  EXPECT_THAT(response.result(), Eq(http::status::internal_server_error));
}

TEST_F(MeterReadingControllerTest, StoreShouldResponseWithErrorGivenEmptyMeterReading) {
  json body = R"({
            "smartMeterId": "smart-meter-0",
            "electricityReadings": []
    })"_json;
  auto req = BuildRequest(http::verb::post, "/readings/store", body);
  std::vector<std::string> queries;

  auto response = controller.Store(req, queries);

  EXPECT_THAT(response.result(), Eq(http::status::internal_server_error));
}

TEST_F(MeterReadingControllerTest, StoreShouldResponseWithErrorGivenNoMeterReadingIsSupplied) {
  json body = R"({
            "smartMeterId": "smart-meter-0"
    })"_json;
  auto req = BuildRequest(http::verb::post, "/readings/store", body);
  std::vector<std::string> queries;

  auto response = controller.Store(req, queries);

  EXPECT_THAT(response.result(), Eq(http::status::internal_server_error));
}

TEST_F(MeterReadingControllerTest, StoreShouldStoreGivenMultipleBatchesOfMeterReadings) {
  json body1 = R"({
            "smartMeterId": "smart-meter-0",
            "electricityReadings": [
                    {
                            "time": "2021-08-18T06:42:15.725202Z",
                            "reading": 1
                    }
            ]
    })"_json;
  json body2 = R"({
            "smartMeterId": "smart-meter-0",
            "electricityReadings": [
                    {
                            "time": "2021-08-18T06:44:15.725202Z",
                            "reading": 2
                    }
            ]
    })"_json;
  auto req1 = BuildRequest(http::verb::post, "/readings/store", body1);
  auto req2 = BuildRequest(http::verb::post, "/readings/store", body2);
  std::vector<std::string> queries;

  controller.Store(req1, queries);
  controller.Store(req2, queries);

  std::vector<ElectricityReading> expectedElectricityReadings = {
      {detail::fromRfc3339("2021-08-18T06:42:15.725202Z"), 1},
      {detail::fromRfc3339("2021-08-18T06:44:15.725202Z"), 2}
  };

  EXPECT_THAT(meterReadingService.getReadings("smart-meter-0"), Eq(expectedElectricityReadings));
}

TEST_F(MeterReadingControllerTest, StoreShouldStoreAssociatedWithUserGivenMeterReadingsAssociatedWithTheUser) {
  json body1 = R"({
            "smartMeterId": "smart-meter-0",
            "electricityReadings": [
                    {
                            "time": "2021-08-18T06:42:15.725202Z",
                            "reading": 1
                    }
            ]
    })"_json;
  json body2 = R"({
            "smartMeterId": "smart-meter-1",
            "electricityReadings": [
                    {
                            "time": "2021-08-18T06:44:15.725202Z",
                            "reading": 2
                    }
            ]
    })"_json;
  auto req1 = BuildRequest(http::verb::post, "/readings/store", body1);
  auto req2 = BuildRequest(http::verb::post, "/readings/store", body2);
  std::vector<std::string> queries;

  controller.Store(req1, queries);
  controller.Store(req2, queries);

  std::vector<ElectricityReading> expectedElectricityReadings = {
      {detail::fromRfc3339("2021-08-18T06:42:15.725202Z"), 1},
  };

  EXPECT_THAT(meterReadingService.getReadings("smart-meter-0"), Eq(expectedElectricityReadings));
}

TEST_F(MeterReadingControllerTest, ReadShouldReturnNotFoundGivenMeterIdThatIsNotRecognised) {
  http::request<http::string_body> req;
  std::vector<std::string> queries = {"smart-meter-0"};

  auto response = controller.Read(req, queries);

  EXPECT_THAT(response.result(), Eq(http::status::not_found));
}