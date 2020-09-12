
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//WIFi init
const char* ssid = "your wifi name";
const char* password = "your wifi password";
WiFiServer server(8000);

//sensor init
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200); //Default Baud Rate for NodeMCU
  delay(10);

  pinMode(D4, OUTPUT);  // Start signal
  digitalWrite(D4, 0);
  pinMode(D6, OUTPUT);  // Stop signal
  digitalWrite(D6, 1);
  delay(500);
  digitalWrite(D6, 0);

  pinMode(D5, INPUT);  // heater state
  pinMode(D3, INPUT);  // manual button

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  Serial.print("macAddress: ");
  Serial.println(WiFi.macAddress());
  Serial.print("AP subnet mask: ");
  Serial.println(WiFi.subnetMask());
  //Start the DS18B20 sensor
  sensors.begin();
}

long test = 0;

void loop() {
  int val, state, button,counter;
  unsigned long start_time, end_time;
  //if button is pressed turn on // if button is pressed for more than 2 secs turn off
  state = digitalRead(D5);
  button = digitalRead(D3);
  if (button == LOW){
    digitalWrite(D4, 1);       //turn on
    delay(500);
    digitalWrite(D4, 0);   //turn off
    counter=0;
    while(button != HIGH){
      counter++;
      Serial.println(counter);
      delay(500);
      button = digitalRead(D3);
      if (counter > 2)
        digitalWrite(D6, 1);   //turn off
        delay(500);
        digitalWrite(D6, 0);   //turn off
    }
  }

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
  //  test++;
  //  if (test > 1000000000) ESP.restart();
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  int max_delay=0;
  while(!client.available()){
    delay(1);
    max_delay++;
    if (max_delay>10000){
      Serial.println("client error");
      ESP.restart();
    }
  }


  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  String s;

  if (req.indexOf("/gpio/0") != -1){
    digitalWrite(D6, 1);       //turn on
    delay(500);
    digitalWrite(D6, 0);   //turn off
  }
  else if (req.indexOf("/gpio/1") != -1){
    digitalWrite(D4, 1);       //turn on
    delay(500);
    digitalWrite(D4, 0);   //turn off
  }
  else if (req.indexOf("/gpio/2") != -1){
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");

    if (state == HIGH) {
     // s = "HTTP/1.1 200 OK\r\nContent-Type: text/html3\r\n\r\n<!DOCTYPE 3>\r\n\r\nGPIO is now ";
     s = "HTTP/1.1 200 OK\r\nContent-Type: text/html3\r\n\r\n";
     s += int(temperatureC);
    }
    else {
     // s = "HTTP/1.1 200 OK\r\nContent-Type: text/html4\r\n\r\n<!DOCTYPE 4>\r\n\r\nGPIO is now ";
     s = "HTTP/1.1 200 OK\r\nContent-Type: text/html4\r\n\r\n";
     s += int(temperatureC);
    }
  }
  else if (req.indexOf("/gpio/3") != -1)
    ESP.restart();
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();


  client.print(s);
  delay(1);
  Serial.println("client disconnected");
}
