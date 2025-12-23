#include "esphome/components/ble_client/ble_client.h"
#include "esphome/core/component.h"

namespace esphome {
namespace shelly_blu_buzzer {

class ShellyBluBuzzer : public Component, public ble_client::BLEClientNode {
 public:
  std::string beacon_uuid = "cb9e957e-952d-4761-a7e1-4416494a5bfa";
  std::string buzzer_uuid = "5b026510-4088-c297-46d8-be6c736a087b";

  void setup() override {
    ESP_LOGI("shelly_blu_buzzer", "Ready to control Shelly BLU Button Tough");
  }

  void enable_beacon() {
    if (!this->client_->is_connected()) return;
    auto chr = this->client_->get_characteristic(this->beacon_uuid);
    if (chr.has_value()) {
      std::vector<uint8_t> data = {1};  // enable beacon mode
      this->client_->write_characteristic(chr.value(), data);
      ESP_LOGI("shelly_blu_buzzer", "Beacon mode enabled");
    }
  }

  void buzz(uint8_t value = 0x01) {
    if (!this->client_->is_connected()) {
      ESP_LOGW("shelly_blu_buzzer", "Not connected");
      return;
    }
    auto chr = this->client_->get_characteristic(this->buzzer_uuid);
    if (chr.has_value()) {
      std::vector<uint8_t> data = {value};
      this->client_->write_characteristic(chr.value(), data);
      ESP_LOGI("shelly_blu_buzzer", "Buzz command sent (value=%d)", value);
    }
  }
};

}  // namespace shelly_blu_buzzer
}  // namespace esphome
