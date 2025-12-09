#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <esp_task_wdt.h>
#include "ConfigSettings.h"
#include "SomfyNetwork.h"
#include "Web.h"
#include "Sockets.h"
#include "Utils.h"
#include "Somfy.h"
#include "MQTT.h"


ConfigSettings settings;
Web webServer;
SocketEmitter sockEmit;
SomfyNetwork net;
rebootDelay_t rebootDelay;
SomfyShadeController somfy;
MQTTClass mqtt;

uint32_t oldheap = 0;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Startup/Boot....");
  Serial.println("Mounting File System...");
  if(LittleFS.begin()) Serial.println("File system mounted successfully");
  else Serial.println("Error mounting file system");
  settings.begin();
  if(WiFi.status() == WL_CONNECTED) WiFi.disconnect(true);
  delay(10);
  Serial.println();
  webServer.startup();
  webServer.begin();
  delay(1000);
  net.setup();  
  somfy.begin();

  // Reconfigure the existing TWDT (already initialized by Arduino framework)
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 7000,
    .idle_core_mask = 1,
    .trigger_panic = true
  };
  esp_task_wdt_reconfigure(&wdt_config);
  esp_task_wdt_add(nullptr);

}

void loop() {
  // put your main code here, to run repeatedly:
  //uint32_t heap = ESP.getFreeHeap();
  if(rebootDelay.reboot && millis() > rebootDelay.rebootTime) {
    Serial.print("Rebooting after ");
    Serial.print(rebootDelay.rebootTime);
    Serial.println("ms");
    net.end();
    ESP.restart();
    return;
  }
  uint32_t timing = millis();
  
  net.loop();
  if(millis() - timing > 100) Serial.printf("Timing Net: %ldms\n", millis() - timing);
  timing = millis();
  esp_task_wdt_reset();
  somfy.loop();
  if(millis() - timing > 100) Serial.printf("Timing Somfy: %ldms\n", millis() - timing);
  timing = millis();
  esp_task_wdt_reset();
  if(net.connected() || net.softAPOpened) {
    webServer.loop();
    esp_task_wdt_reset();
    if(millis() - timing > 100) Serial.printf("Timing WebServer: %ldms\n", millis() - timing);
    esp_task_wdt_reset();
    timing = millis();
    sockEmit.loop();
    if(millis() - timing > 100) Serial.printf("Timing Socket: %ldms\n", millis() - timing);
    esp_task_wdt_reset();
    timing = millis();
  }
  if(rebootDelay.reboot && millis() > rebootDelay.rebootTime) {
    net.end();
    ESP.restart();
  }
  esp_task_wdt_reset();
}
