#include <Arduino.h>
// #include <webpages.h>

// ***** MODULE TESTS: ****
// #define  LED_TEST
// #define  RGB_LED_TEST
// #define SERVO_TEST 
// #define HALL_TEST 
// #define CURRENT_TEST
// #define WEBPAGE_TEST_1
#define WEBPAGE_TEST_2
// #define WEBPAGE_TEST

//====================================================================================================
// START PROTOTYPES 
//====================================================================================================
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
  const int MAGNETS_PER_REV = 1;
  volatile int pulses = 0;
  void onPulse(void) {
    pulses++;
  }
#endif

#if defined(CURRENT_TEST)
  #include <Wire.h>
  #include <Adafruit_INA219.h>
  Adafruit_INA219 ina219;
#endif

#if defined(WEBPAGE_TEST_1)
  #include <WiFi.h>
  #include <time.h>
  const char* ssid = "Phone2";
  const char* password = "1ue4kcmif50fo";

  // NTP server:
  const char* ntpServer = "pool.ntp.org";

  // Timezone offset (seconds)
  // Example: UTC +5:30 = 5.5 * 3600 = 19800
  const long gmtOffset_sec = 0;     // Change for your timezone
  const int daylightOffset_sec = 0; // Change if DST applies
#endif

#if defined(WEBPAGE_TEST_2)
  #include <WiFi.h>
  #include <WebServer.h>
  #include <time.h>

  const char* ssid = "Phone2";
  const char* password = "1ue4kcmif50fo";

  // NTP server:
  const char* ntpServer = "pool.ntp.org";

  // Timezone offset (seconds)
  // Example: UTC +5:30 = 5.5 * 3600 = 19800
  const long gmtOffset_sec = 0;     // Change for your timezone
  const int daylightOffset_sec = 0; // Change if DST applies

  WebServer server(80);

  String formatTimeNow();
  void handleRoot();
  void handleTime();
#endif

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

//====================================================================================================
// END OF PROTOTYPES & START OF MAIN SETUP FUNCTION 
//====================================================================================================

void setup() {
  #if defined(LED_TEST)
    pinMode(EXTERN_PIN, OUTPUT);
  #endif

  #if defined(RGB_LED_TEST)
    Serial.begin(115200);
    pixels.begin();
  #endif 

  #if defined(SERVO_TEST)
    Serial.begin(115200);
    servo.attach(SERVO_PIN);
  #endif

  #if defined(HALL_TEST)
    Serial.begin(115200);
    pinMode(HALL_SENSOR, INPUT);
    attachInterrupt(digitalPinToInterrupt(HALL_SENSOR), onPulse, FALLING);
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

  #if defined(WEBPAGE_TEST_1)
    Serial.begin(115200);
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi Connected");

    // Initialize time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println("Time initialized");
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

//====================================================================================================
// START OF MAIN LOOP FUNCTION
//====================================================================================================

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
      pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      pixels.show();
      delay(DELAY_VAL);
    }
    Serial.println("MOWWW");
    delay(1000);
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
    noInterrupts();
    int count = pulses;
    pulses = 0;
    interrupts();
    
    // RPM = (pulses per second x 60) / magnets per revolution
    float rpm = (count / (float)MAGNETS_PER_REV) * 60.0;
    Serial.print("RPM: ");
    Serial.println(rpm);
    delay(1000);
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

  #if defined(WEBPAGE_TEST_1)
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time!");
      delay(1000);
      return;
    }

    Serial.printf("Date: %04d-%02d-%02d  ",
                   timeinfo.tm_year + 1900,
                   timeinfo.tm_mon + 1,
                   timeinfo.tm_mday);
    Serial.printf("Time: %02d:%02d:%02d\n",
                   timeinfo.tm_hour,
                   timeinfo.tm_min,
                   timeinfo.tm_sec);
    delay(1000); // Update every second
  #endif

  #if defined(WEBPAGE_TEST_2)
      Serial.begin(115200);

      // Connect Wi-Fi
      WiFi.begin(ssid, password);
      Serial.print("Connecting to WiFi");
      while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("\nWiFi connected!");
      Serial.print("ESP32 IP address: ");
      Serial.println(WiFi.localIP());

      // NTP time init
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

      // Optional: wait for a bit for first sync
      Serial.println("Syncing time...");
      for (int i = 0; i < 15; i++) {
        if (formatTimeNow().indexOf("not available") == -1) break;
        delay(500);
      }
      Serial.println("Current time: " + formatTimeNow());
  
      // Web routes
      server.on("/", handleRoot);
      server.on("/time", handleTime);

      server.begin();
      Serial.println("Web server started. Open the IP in your browser.");
  #endif

  #if defined(WEBPAGE_TEST)
    ws.cleanupClients();
    digitalWrite(ledPin, ledState);
  #endif
}

//====================================================================================================
// END OF MAIN LOOP FUNCTION & START OF FUNCTION DECLARATIONS
//====================================================================================================

#if defined(WEBPAGE_TEST_2)
  String formatTimeNow() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) {
      return String("Time not available (NTP not synced yet)");
    }

    char buf[32];

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return String(buf);
  }

  void handleRoot() {
  // Simple HTML page that fetches /time every second
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>ESP32 Time</title>
      <style>
        body { font-family: Arial, sans-serif; margin: 24px; }
        .card { padding: 16px; border: 1px solid #ddd; border-radius: 12px; max-width: 420px; }
        #t { font-size: 1.6rem; font-weight: 700; }
        .small { color: #666; margin-top: 8px; }
      </style>
    </head>
    <body>
      <div class="card">
        <div>Current time:</div>
        <div id="t">Loading...</div>
        <div class="small">Updates every second</div>
      </div>

      <script>
        async function updateTime(){
          try{
            const r = await fetch('/time', { cache: 'no-store' });
            const text = await r.text();
            document.getElementById('t').textContent = text;
          } catch(e){
            document.getElementById('t').textContent = 'Error';
          }
        }
        updateTime();
        setInterval(updateTime, 1000);
      </script>
    </body>
    </html>
    )rawliteral";

    server.send(200, "text/html", html);
  }

  void handleTime() {
    server.send(200, "text/plain", formatTimeNow());
  }
#endif

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