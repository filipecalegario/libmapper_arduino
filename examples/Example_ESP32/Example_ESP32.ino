#include <WiFi.h>
#include "mapper.h"

const char* ssid     = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

mapper_device dev = 0;
mapper_signal input_signal = 0;
mapper_signal output_signal = 0;
float seq_number = 0;
float received_value = 0;

void setup() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  float min = 0.0f;
  float max = 5.0f;

  dev = mapper_device_new("ESP32", 0, 0);
  output_signal = mapper_device_add_output_signal(dev, "value_to_send", 1, 'f', "V", &min, &max);
  input_signal = mapper_device_add_input_signal(dev, "value_received", 1, 'f', 0, &min, &max, input_signal_handler, 0);
}

void loop() {
  mapper_device_poll(dev, 120);
  seq_number = seq_number + 0.01f;
  mapper_signal_update_float(output_signal, seq_number);
}

void input_signal_handler(mapper_signal sig, mapper_id instance, const void *value, int count, mapper_timetag_t *timetag) {
  if (value) {
    float *v = (float*)value;
    for (int i = 0; i < mapper_signal_length(sig); i++) {
      received_value = v[i];
    }
  }
}
