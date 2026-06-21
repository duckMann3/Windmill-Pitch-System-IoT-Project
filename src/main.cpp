#include <Arduino.h>
#include <webpages.h>

// ***** MODULE TESTS: ****
// #define  LED_TEST
// #define  RGB_LED_TEST
// #define SERVO_TEST 
// #define HALL_TEST 
// #define CURRENT_TEST
#define WEBPAGE_TEST

#if defined(LED_TEST)
  #define  EXTERN_PIN     7   // External RED LED
#endif

#if defined(RGB_LED_TEST)
  #include <Adafruit_NeoPixel.h>
  #define  ON_BOARD_PIN   48  // On-board RGB LED Pin
  #define  NUM_PIXELS     1   // Number of Pixels for RGB LED
  #define  DELAY_VAL      500 // Delay
  Adafruit_NeoPixel pixels(NUM_PIXELS, ON_BOARD_PIN, NEO_GRB + NEO_KHZ800);
#endif

#if defined(SERVO_TEST)
  #include <ESP32Servo.h>
  static const int SERVO_PIN = 4;
  #define MIN 600
  #define MAX 2400
  Servo servo;
#endif

#if defined(HALL_TEST)
  #define HALL_SENSOR 11
  volatile int count = 0; 
#endif

#if defined(CURRENT_TEST)
  #include <Wire.h>
  #include <Adafruit_INA219.h>
  Adafruit_INA219 ina219;
#endif

#if defined(WEBPAGE_TEST)
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>

  // Function Prototypes:
  void notifyClients();
  void handleWebSocketMessage(void* arg, uint8_t* data, size_t len);
  void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);
  void initWebSocket();
  String processor(const String& var);

  const char* ssid = "REPLACE";
  const char* password = "REPLACE";

  bool ledState = 0;
  const int ledPin = 2;

  // Create AsyncWebServer object on port 80
  AsyncWebServer server(80);
  AsyncWebSocket ws("/ws");
#endif

void setup() {
  #if defined(LED_TEST)
    pinMode(EXTERN_PIN, OUTPUT);
  #endif

  #if defined(RGB_LED_TEST)
    pixels.begin();
  #endif 

  #if defined(SERVO_TEST)
    Serial.begin(115200);
    servo.attach(SERVO_PIN);
  #endif

  #if defined(HALL_TEST)
    Serial.begin(115200);
    pinMode(HALL_SENSOR, INPUT);
  #endif

  #if defined(CURRENT_TEST)
    Serial.begin(115200);
    while(!Serial) {
      delay(1);
    }

    if(!ina219.begin()) {
      Serial.println("Failed to find INA219 chip");
      while(1) {
        delay(10);
      }
    }
  #endif

  #if defined(WEBPAGE_TEST)
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Connect to Wi-Fi:
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }

    // Print ESP Local IP Address:
    Serial.println(WiFi.localIP());

    initWebSocket();

    // Route for root / webpage
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
      request -> send(200, "text/html", index_html, processor);
    });

    server.begin(); // Start server
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  #if defined(LED_TEST)
    digitalWrite(EXTERN_PIN, HIGH);
    delay(1000);
    digitalWrite(EXTERN_PIN, LOW);
    delay(1000);
  #endif

  #if defined(RGB_LED_TEST)
    pixels.clear();
    for(int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAY_VAL);
    }
  #endif

  #if defined(SERVO_TEST)
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) { 
      servo.write(posDegrees);
      Serial.println(posDegrees);
      delay(20);
    }
    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) { 
      servo.write(posDegrees);
      Serial.println(posDegrees);
      delay(20);
    }
  #endif

  #if defined(HALL_TEST)
    if(digitalRead(HALL_SENSOR) == LOW) {
      count++;
    }
    Serial.print("Magnet passes: ");
    Serial.println(count);
    delay(500);
  #endif

  #if defined(CURRENT_TEST)
    float shuntVoltage = 0; // I forgot what shunt voltage is!
    float busVoltage = 0;   // I'm guessing for the I2C bus
    float current_mA = 0;   // Variable stores current measurements?
    float loadVoltage = 0;  // I don't know what hte load voltage is? 
    // float power_mW = 0;     // Variable stores power measurements

    Serial.println("Measuring voltage, current, & power ... ");
    shuntVoltage = ina219.getShuntVoltage_mV();
    busVoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadVoltage = busVoltage + (shuntVoltage / 1000);
    Serial.print("Bus Voltage: "); Serial.print(busVoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shuntVoltage); Serial.println(" mV");
    Serial.print("Load Voltage: "); Serial.print(loadVoltage); Serial.println(" V");
    Serial.print("Current: "); Serial.print(current_mA); Serial.println(" mA");
    // Serial.print("Power: "); Serial.print(power_mW); Serial.println(" mW");
    delay(500);
  #endif

  #if defined(WEBPAGE_TEST)
    ws.cleanupClients();
    digitalWrite(ledPin, ledState);
  #endif
}

#if defined(WEBPAGE_TEST)
  void notifyClients() {
    ws.textAll(String(ledState));
  }

  void handleWebSocketMessage(void* arg, uint8_t* data, size_t len) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if(info -> final && info -> index == 0 && info -> len == len && info -> opcode == WS_TEXT) {
      data[len] = 0;
      if(strcmp((char*)data, "toggle") == 0) {
        ledState = !ledState;
        notifyClients();
      }
    }
  }

  void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch(type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client -> id(), client -> remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client $%u disconnected\n", client -> id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
    }
  }

  void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
  }

  String processor(const String& var) {
    Serial.println(var);
    if(var == "STATE") {
      if(ledState) {
        return "ON";
      } else {
        return "OFF";
      }
    }
    return String();
  }
#endif