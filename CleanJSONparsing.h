// expected return from 
// http://worldclockapi.com/api/json/est/now:
/* {
  "$id":"1",
  "currentDateTime":"2019-07-26T08:47-04:00",
  "utcOffset":"-04:00:00",
  "isDayLightSavingsTime":true,
  "dayOfTheWeek":"Friday",
  "timeZoneName":"Eastern Standard Time",
  "currentFileTime":132086044656636912,
  "ordinalDate":"2019-207",
  "serviceResponse":null
  }
*/

//this relies on ArduinoJson 5.13

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
 
const char* wifiName     = "buttfuzz";
const char* wifiPass = "rocket18"; 
 
//Web Server address to read/write from 
const char *host = "http://worldclockapi.com/api/json/est/now";
 
void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(wifiName);
 
  WiFi.begin(wifiName, wifiPass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
}
 
void loop() {
  HTTPClient http;    //Declare object of class HTTPClient
 
  Serial.print("Request Link:");
  Serial.println(host);
  
  http.begin(host);     //Specify request destination
  
  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server
 
  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code
 
  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload
  
  if(httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(9) + 350;
    DynamicJsonBuffer jsonBuffer(capacity);
  
   // Parse JSON object
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
  
    // Decode JSON/Extract values
	// For data in arrays, try this --> Serial.println(root["isDayLightSavingsTime"][1][2].as<char*>());
    Serial.println(F("Response:"));
    Serial.println(root["currentDateTime"].as<char*>());
    Serial.println(root["isDayLightSavingsTime"].as<char*>());
    Serial.println(root["dayOfTheWeek"].as<char*>());
    Serial.println(root["currentFileTime"].as<char*>());
  }
  else
  {
    Serial.println("Error in response");
  }
 
  http.end();  //Close connection
  
  delay(5000);  //GET Data at every 5 seconds
}