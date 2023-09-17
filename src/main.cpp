#include <Arduino.h>
#include <RBDdimmer.h> // https://github.com/RobotDynOfficial/RBDDimmer

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#include <ArduinoJson.h>

const bool DEBUG = false;

// ! DIMMER SETUP ---------------------------------------------------------------------------------------------
/*
    ? == you can choose the number
    connect the sync pin of the circuit to the esp32's GPIO pin       sync -> GPIO?
    connect 4 wires from potentiometer to esp32 pins                  potentiometer -> GPIO?  * 4
    connect 4 wires from the ch1-ch4 of the circuit to esp32 pins     ch? -> GPIO?  * 4
*/
const byte POTENTIOIMETER_PINS[] = {13, 26, 14, 27}; // numbers and amount of potentiometers changeable
const byte DIMMER_PINS[] = {32, 19, 5, 18};          // numbers and amount of channels changeable
const byte INTERRUPT_PIN = 34;

const byte NUM_OF_LIGHTS = 3; // changeable
const byte MIN_LIGHT = 0;     // the minimum percentage of power that goes to the lamps    changeable
const byte MAX_LIGHT = 90;    // the maximum percentage of power that goes to the lamps    changeable

dimmerLamp dimmers[] = {
    dimmerLamp(DIMMER_PINS[0], INTERRUPT_PIN),
    dimmerLamp(DIMMER_PINS[1], INTERRUPT_PIN),
    dimmerLamp(DIMMER_PINS[2], INTERRUPT_PIN),
    dimmerLamp(DIMMER_PINS[3], INTERRUPT_PIN)}; // numbers and amount of lamps changeable

byte values[4]; // percentage of power [0,100]

// ! WIFI SETUP ------------------------------------------------------------------------------------------------
const char *ssid = "SSID";
const char *password = "PASS";

const char *apiUrl = "https://ets.leontomic.com/";
const char *jwtToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VySWQiOiIwdW1mOWJyWmJ1RlFiLXY2WU1xR00iLCJpYXQiOjE2OTQ5NzI2NTMsImV4cCI6MzQwNTQ5NzMwNn0.HrE7Rd7B41CVea-pW_974ES61_ZgxsU1FoFH7AJSACI"; // Zamijeni sa JWT tokenom
const char *lightIds[] = {"5WQmn8wVvV0dts72XwWJxLUf", "9TIJrTD6XLYd2iCOU1VsEEl7", "L8yU944S4-fxt_jxEkYQQvO1"};                                                                                         // Zamijeni s ID-em svijetla

const int httpsPort = 443; // HTTPS port

WiFiMulti wifiMulti;

bool testApi()
{
  return false;
}
// promijeni ovu funkciju u svoju
void get_values()
{
  for (byte i = 0; i < NUM_OF_LIGHTS; i++)
  {
    values[i] = map(analogRead(POTENTIOIMETER_PINS[i]), 1, 4095, MIN_LIGHT, MAX_LIGHT);

    // debugging
    Serial.print(analogRead(POTENTIOIMETER_PINS[i]));
    Serial.print(" = ");
    Serial.print(values[i]);
    Serial.print(" is ");
    Serial.print(dimmers[i].getPower());
    Serial.println();
  }
  Serial.println();
}

void setup()
{
  Serial.begin(9600);

  // ! DIMMER SETUP
  for (byte i = 0; i < NUM_OF_LIGHTS; i++)
  {
    // setup dimmers
    dimmers[i].begin(NORMAL_MODE, ON);
    pinMode(DIMMER_PINS[i], OUTPUT);

    // setup potentiometer pins
    pinMode(POTENTIOIMETER_PINS[i], INPUT);
  }

  // ! WIFI SETUP
  wifiMulti.addAP("MaliMrki", "Bubamara13");
  /*while (!testApi())
  {
    Serial.println("Something is wrong with the API...");
    delay(5000);
  }*/
}

int parseJson(String input)
{
  StaticJsonDocument<192> doc;
  DeserializationError error = deserializeJson(doc, input);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return -1;
  }

  JsonObject light = doc["light"];
  int light_on = light["on"];
  int light_dimness = light["dimness"];

  if (!light_on)
  {
    return 0;
  }
  return light_dimness;
}

int fetchData(const char *lightID)
{
  if ((wifiMulti.run() == WL_CONNECTED))
  {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");

    String fullUrl = String(apiUrl) + String("lights/") + String(lightID);
    http.begin(fullUrl);
    Serial.print("[HTTP] GET ");
    Serial.println(fullUrl);

    http.setAuthorizationType("Bearer");
    http.setAuthorization(jwtToken);

    int httpCode = http.GET();

    int lightDimness = 0;
    if (httpCode > 0)
    {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        String payload = http.getString();
        Serial.println(payload);
        lightDimness = parseJson(payload);
      }
      else
      {
        String payload = http.getString();
        Serial.println(payload);

        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    }
    return lightDimness;
  }
  return 0;
}
void loop()
{
  // get the values
  if (DEBUG)
  {
    get_values();
  }
  else
  {
    // delay(5000);
    for (int i = 0; i < NUM_OF_LIGHTS; i++)
    {
      int dimness = fetchData(lightIds[i]);
      if (dimness == -1)
      {
        values[i] = 0;
        Serial.printf("Something went wrong with %dth light", i);
      }
      else
      {
        values[i] = dimness > MAX_LIGHT ? 90 : dimness;
        dimmers[i].setPower(values[i]);
      }
    }
  }

  // set the values
  if (DEBUG)
  {
    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
      dimmers[i].setPower(values[i]);
    }
  }
}