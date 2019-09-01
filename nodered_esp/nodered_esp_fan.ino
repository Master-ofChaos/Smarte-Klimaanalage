#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


int pwmFanRpm = 1023;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);

  // Initialisieren
  pinMode(D2, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("betahaus2.0", "betahaus10?");
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    //Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://172.16.106.48:1880/Fan")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          pwmFanRpm = payload.toInt();
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  //---------------FAN-------------
  //analogWrite(D2, mapfloat(constrain(pwmFanRpm, 0, 255), 0, 255, 0.99, 2));    // start PWM
  analogWrite(D2,constrain(pwmFanRpm, 0, 1023));
}

/*float mapfloat(int x, int in_min, int in_max, int out_min, int out_max)
{
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}*/
