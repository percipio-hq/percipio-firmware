#pragma once

struct SensorReading {
  float temperature; // °C
  float humidity;    // %
  float pressure;    // hPa
};

struct RadarTarget {
  int   x;     // mm
  int   y;     // mm
  float speed; // m/s
};

struct RadarData {
  int         count; // 0–3
  RadarTarget targets[3];
};

struct RfidEvent {
  const char* uid;
  const char* name;
  bool        authorized;
};

struct DeviceStatus {
  bool bme280_ok;
  bool rfid_ok;
  bool radar_ok;
};

void firebaseInit();
bool firebaseSendReading(const SensorReading& r);
bool firebaseSendRadar(const RadarData& d);
bool firebaseSendRfid(const RfidEvent& e);
bool firebaseSendStatus(const DeviceStatus& s);
