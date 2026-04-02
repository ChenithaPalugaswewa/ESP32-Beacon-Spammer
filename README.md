# ESP32 Beacon Spammer

## Overview
The **Beacon Spammer** is a specialized diagnostic tool for ESP32 hardware designed to demonstrate the architectural limitations of WiFi scanning on mobile operating systems. By utilizing raw 802.11 frame injection, the device generates a saturated environment of non-existent Access Points (APs). 

This project is specifically optimized for stress-testing WiFi scan lists on legacy Android and modern iOS devices.

## Technical Mechanism
The implementation operates at the **Layer 2 Data Link layer**. It bypasses the standard high-level WiFi management stack (L3+) to eliminate the processing overhead associated with traditional Access Point initialization and beaconing.

### Core Architecture:
* **Frame Control:** Targets Management Type `0x80` (Beacon Frame) to ensure maximum visibility to scanning stations.
* **Sequence Control:** Manually increments the 12-bit sequence number (Bytes 22-23). This is the primary mechanism used to bypass "De-duplication" logic in mobile WiFi drivers, preventing the OS from merging repeated frames.
* **Channel Hopping:** Rapidly switches the physical radio frequency across the 2.4GHz ISM band (Channels 1-11) to ensure saturation across the entire scan spectrum.
* **MAC Spoofing:** Randomizes the BSSID and Source MAC for every injected packet, forcing the receiving device to allocate a unique memory slot for each SSID in the UI.

## Usage
1. Open the project in the **Arduino IDE**.
2. Ensure the **ESP32 Board Manager** is installed.
3. Flash the `.ino` source code to your ESP32-based hardware (e.g., Magicbit).
4. Monitor transmission status via the Serial Monitor at **115200 baud**.

## Disclaimer
This tool is intended for educational purposes and authorized security research within shielded, controlled environments. Generating excessive radio frequency interference may violate local telecommunications regulations. Use responsibly.
