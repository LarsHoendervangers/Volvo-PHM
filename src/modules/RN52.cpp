#include "RN52.h"

createSafeString(buffer, 256);
createSafeString(config, 20 * 18);

RN52::RN52() {}

void RN52::begin() {
  // I have set my module to this speed before, default is 115200.
  Serial2.begin(9600);
  Serial.println("RN52 begin");
  requestConfigUpdate();
  SafeString::setOutput(Serial);
}

void RN52::setDiscoverable(bool discoverable) {
  if (discoverable) Serial2.print("@,1\r");
  else Serial2.print("@,0\r");
}

void RN52::toggleCommandEcho() {
  Serial2.print("+\r");
}

void RN52::requestConfigUpdate() {
  Serial2.print("D\r");
}

void RN52::requestAGBatteryLevel() {
  Serial2.print("GB\r");
}

int lastBufferUpdate = 0;
bool hasBufferBeenProcessed = false;
void RN52::update() {
  unsigned long currentMillis = millis();
  while (Serial2.available() > 0) {
    char read = (char) Serial2.read();
    if (!buffer.isFull()) buffer += read;
    lastBufferUpdate = currentMillis;
    hasBufferBeenProcessed = false;
  }

  // If no data has been received for 2 milliseconds, assume that data should be processed.
  if (currentMillis - lastBufferUpdate > 2 && !hasBufferBeenProcessed) {
    hasBufferBeenProcessed = true;
    processBuffer();
  }
}

AudioGateway RN52::getAG() {
  return this->ag;
}

void RN52::processBuffer() {
  if (buffer.length() == 0) return;

  if (buffer.startsWith("*** Settings ***")) { // 'D' command
    Serial.println("D Command");
    int btacIndex = buffer.indexOf("BTAC") + 5;
    if (btacIndex >= 0) { // BTAC
      for (int i = btacIndex; i < btacIndex + 12; i++) {
        this->ag.connectedMAC[i - btacIndex] = buffer.charAt(i);
      }
    }
  } else if (buffer.startsWith("AGBatteryLevel")) { // 'GB' command
    Serial.println("GB Command");
    int index = buffer.indexOf("=") + 1;
    int endIndex = buffer.indexOf("%");
    char battery[3];
    for (int i = index; i < endIndex; i++) {
      battery[i - index] = buffer.charAt(i);
    }
    this->ag.batteryLevel = atoi(battery);
  }

  buffer.clear();
}