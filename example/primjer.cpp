#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPAsyncWebClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const char* apiUrl = "https://uuedimajdimaj.ddns.net/lights/";
const char* jwtToken = "AUTH_TOKEN";  // Zamijeni sa JWT tokenom
const char* lightId = "xyz";  // Zamijeni s ID-em svijetla

const int httpsPort = 443;  // HTTPS port

WiFiClientSecure client;
AsyncWebClient webClient(client);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Fetch data from the API
  fetchData();
}

void fetchData() {
  if (WiFi.status() == WL_CONNECTED) {
    String endpoint = apiUrl;
    endpoint += lightId;

    Serial.print("Connecting to: ");
    Serial.println(endpoint);

    webClient.begin(endpoint, httpsPort);

    // Set headers
    webClient.addHeader("Authorization", "Bearer " + String(jwtToken));

    // Make the GET request
    webClient.get(endpoint, [](AsyncWebClientResponse * response) {
      String payload = response->getString();
      Serial.println("Response:");
      Serial.println(payload);

      // Parse the JSON response here and handle the data as needed
      // Example: Extract "status," "on," and "dimness" values from the JSON response
    });
  } else {
    Serial.println("WiFi not connected");
  }
}

void loop() {
  // You can add other code or functionality here
}
