#include "firebase.h"
#include "config.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

#define FIRESTORE_BASE \
  "https://firestore.googleapis.com/v1/projects/" \
  FIREBASE_PROJECT_ID \
  "/databases/(default)/documents/"

static String timestamp() {
  time_t now;
  time(&now);
  char buf[30];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
  return String(buf);
}

static bool postDocument(const char* collection, const String& body) {
  WiFiClientSecure client;
  client.setInsecure(); // dev only — add root CA cert before production

  HTTPClient http;
  String url = String(FIRESTORE_BASE) + collection + "?key=" + FIREBASE_API_KEY;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  int code = http.POST(body);
  bool ok  = (code == 200);

  if (ok)   Serial.printf("[Firebase] %s sent\n", collection);
  else      Serial.printf("[Firebase] %s failed (%d): %s\n",
                          collection, code, http.getString().c_str());
  http.end();
  return ok;
}

void firebaseInit() {
  // Sync NTP time — required for accurate Firestore timestamps
  configTime(0, 0, "pool.ntp.org", "time.google.com");
  Serial.print("[Firebase] syncing time");
  time_t now = 0;
  while (now < 1000000000UL) { delay(500); Serial.print("."); time(&now); }
  Serial.println(" OK");
}

bool firebaseSendReading(const SensorReading& r) {
  StaticJsonDocument<256> doc;
  JsonObject f = doc.createNestedObject("fields");
  f["temperature"]["doubleValue"] = r.temperature;
  f["humidity"]["doubleValue"]    = r.humidity;
  f["pressure"]["doubleValue"]    = r.pressure;
  f["created_at"]["timestampValue"] = timestamp();

  String body;
  serializeJson(doc, body);
  return postDocument("sensor_readings", body);
}

bool firebaseSendRadar(const RadarData& d) {
  StaticJsonDocument<512> doc;
  JsonObject f = doc.createNestedObject("fields");
  f["target_count"]["integerValue"] = String(d.count);
  f["created_at"]["timestampValue"] = timestamp();

  JsonArray arr = f["targets"]["arrayValue"]["values"].to<JsonArray>();
  for (int i = 0; i < d.count; i++) {
    JsonObject entry = arr.createNestedObject();
    JsonObject mf    = entry["mapValue"]["fields"].to<JsonObject>();
    mf["x"]["integerValue"] = String(d.targets[i].x);
    mf["y"]["integerValue"] = String(d.targets[i].y);
    mf["speed"]["doubleValue"] = d.targets[i].speed;
  }

  String body;
  serializeJson(doc, body);
  return postDocument("radar_targets", body);
}

bool firebaseSendStatus(const DeviceStatus& s) {
  StaticJsonDocument<256> doc;
  JsonObject f = doc.createNestedObject("fields");
  f["online"]["booleanValue"]     = true;
  f["bme280_ok"]["booleanValue"]  = s.bme280_ok;
  f["rfid_ok"]["booleanValue"]    = s.rfid_ok;
  f["radar_ok"]["booleanValue"]   = s.radar_ok;
  f["updated_at"]["timestampValue"] = timestamp();

  String body;
  serializeJson(doc, body);
  return postDocument("device_status", body);
}

bool firebaseSendRfid(const RfidEvent& e) {
  StaticJsonDocument<256> doc;
  JsonObject f = doc.createNestedObject("fields");
  f["card_uid"]["stringValue"]        = e.uid;
  f["card_name"]["stringValue"]       = e.name;
  f["authorized"]["booleanValue"]     = e.authorized;
  f["created_at"]["timestampValue"]   = timestamp();

  String body;
  serializeJson(doc, body);
  return postDocument("rfid_events", body);
}
