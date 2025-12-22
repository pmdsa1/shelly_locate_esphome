#pragma once

#include "esphome.h"
#include "esphome/components/esp32_ble_client/ble_client.h"

class ShellyLocate : public esphome::Component,
                     public esphome::esp32_ble_client::BLEClientBase {
 public:
  // MAC address of your Shelly BLU Button Tough
  std::string mac = "7C:C6:B6:74:F1:E2";

  // Shelly BLU service + characteristic UUIDs
  esphome::esp32_ble_client::BLEUUID service_uuid{
      "de8a5aac-a99b-c315-0c80-60d4cbb51225"};
  esphome::esp32_ble_client::BLEUUID char_uuid{
      "5b026510-4088-c297-46d8-be6c736a087b"};

  // Called by the automation action
  void activate_buzzer() {
    ESP_LOGI("shelly_locate", "Connecting to Shelly BLU...");
    this->connect();  // async, non-blocking
  }

 protected:
  // BLE device filter (only match our Shelly)
  bool parse_device(const esphome::esp32_ble_tracker::ESPBTDevice &device) override {
    return device.address_str() == mac;
  }

  // Called when BLE connection succeeds
  void on_connect() override {
    ESP_LOGI("shelly_locate", "Connected!");

    auto *service = this->client_->get_service(service_uuid);
    if (!service) {
      ESP_LOGE("shelly_locate", "Service not found");
      this->disconnect();
      return;
    }

    auto *chr = service->get_characteristic(char_uuid);
    if (!chr) {
      ESP_LOGE("shelly_locate", "Characteristic not found");
      this->disconnect();
      return;
    }

    // Shelly BLU buzzer command
    uint8_t payload[3] = {0x01, 0x00, 0x01};

    ESP_LOGI("shelly_locate", "Writing buzzer command...");
    bool ok = chr->write_value(payload, sizeof(payload), false);

    if (ok)
      ESP_LOGI("shelly_locate", "Buzzer activated!");
    else
      ESP_LOGE("shelly_locate", "Write failed");

    this->disconnect();
  }

  // Called when connection fails
  void on_connect_failed() override {
    ESP_LOGE("shelly_locate", "Failed to connect");
  }

  // Called when disconnected
  void on_disconnect() override {
    ESP_LOGI("shelly_locate", "Disconnected");
  }
};

//
// Automation Action Class
//
class ShellyLocateActivateAction : public esphome::automation::Action {
 public:
  void set_parent(ShellyLocate *parent) { parent_ = parent; }

  void play(esphome::automation::ActionContext &) override {
    parent_->activate_buzzer();
  }

 protected:
  ShellyLocate *parent_;
};
