# Project 02: IoT-Based Solar Panel Real-Time Monitoring System

## 📌 Overview
An end-to-end IoT embedded telemetry system engineered to acquire, calculate, and transmit photovoltaic performance metrics in real time. The platform continuously monitors actual solar power generation versus theoretical model expectations to identify dust coverage, shading, temperature degradation, and equipment faults.

---

## ⚡ System Architecture

```
[ Solar Panel ]
       │
       ├───> [ Voltage Divider (23:1) ] ─────────> ESP32 ADC (Pin 35)
       ├───> [ ACS712 Current Sensor ] ──────────> ESP32 ADC (Pin 33)
       ├───> [ Irradiance Sensor (Pyranometer) ] ─> ESP32 ADC (Pin 34)
       └───> [ DS18B20 Temp Sensor ] ───────────> ESP32 ADC (Pin 32)
                                                         │
                                                  [ ESP32 MCU ]
                                                  (FreeRTOS / C++)
                                                         │
                                        ┌────────────────┴───────────────┐
                                        ▼                                ▼
                            [ Local HTTP Web Server ]        [ Google Firebase RTDB ]
                            (Zero-latency local diagnostics) (Cloud telemetry & analytics)
```

---

## 🛠️ Key Hardware & Firmware Features

1. **Precision Analog Signal Conditioning**:
   * Hardware voltage divider stepped down with a 23.0 ratio to safely interface solar panel voltage with the ESP32 3.3V ADC.
   * ACS712 Hall-effect current sensor with 200-sample averaging calibration on boot to cancel DC offset and operational amplifier drift.
   * Irradiance sensing calibrated up to $1200\text{ W/m}^2$.
2. **Dual-Mode Telemetry & Communications**:
   * **Cloud Streaming**: Secure authenticated client streaming data straight to Google Firebase Realtime Database.
   * **Local Embedded Web Server**: Serves responsive status HTML directly from the ESP32 for standalone field troubleshooting without internet access.
3. **Signal & Data Analytics**:
   * MATLAB scripts (`analysis/signal_analysis.m`) perform FFT spectral analysis and resample sensor streams to evaluate power-frequency fluctuations and ripple.

---

## 📂 Source Code & Assets

* [`firmware/solar_dashboard.ino`](firmware/solar_dashboard.ino): Complete ESP32 Arduino/C++ firmware.
* [`analysis/signal_analysis.m`](analysis/signal_analysis.m): Signal processing and FFT analysis script in MATLAB.
* [`analysis/report.pdf`](analysis/report.pdf): Academic technical evaluation report.
