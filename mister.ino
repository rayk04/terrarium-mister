#include <DHT.h>
#include <DHT_U.h>
#include <L298N.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define DHTPIN 4 // GPIO 4 - D2
#define DHTTYPE DHT11

// WiFi credentials
const char* ssid = "memes";
const char* password = "applepie180";

// Motor control pins
const int IN1 = 14; // GPIO 14 - D5
const int IN2 = 12; // GPIO 12 - D6
const int EN = 5;   // GPIO 5 - D1
L298N motor(EN, IN1, IN2);

// DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Web server
ESP8266WebServer server(80);

// Motor state and target humidity
bool targetSet = false;    // User must set target before control starts
float targetHumidity = 0;  // Default is unset

// Timing for humidity check
unsigned long lastCheck = 0;
const unsigned long checkInterval = 2000; // 2 seconds

// Function to control the motor
void setMotorState(bool state) {
  if (state) {
    motor.forward();
    Serial.println("Motor turned ON");
  } else {
    motor.stop();
    Serial.println("Motor turned OFF");
  }
}

// Web page handler
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Humidity Control</title>
    </head>
    <body>
      <h1>Humidity Control</h1>
      <p>Current humidity: <b>)rawliteral" + String(dht.readHumidity()) + R"rawliteral(%</b></p>
      <p>Target humidity: <b>)rawliteral" + (targetSet ? String(targetHumidity) + "%" : "Not Set") + R"rawliteral(</b></p>
      <form action="/set" method="POST">
        <label for="target">Set Target Humidity (%):</label>
        <input type="number" id="target" name="target" step="1" min="0" max="100" required>
        <button type="submit">Set</button>
      </form>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// Set target humidity handler
void handleSetTarget() {
  if (server.hasArg("target")) {
    float newTarget = server.arg("target").toFloat();
    if (newTarget >= 0 && newTarget <= 100) {
      targetHumidity = newTarget;
      targetSet = true; // Enable motor control
      server.send(200, "text/plain", "Target humidity updated to " + String(targetHumidity) + "%");
    } else {
      server.send(400, "text/plain", "Bad Request: Target must be between 0 and 100%");
    }
  } else {
    server.send(400, "text/plain", "Bad Request: Missing target value");
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Initialize motor
  motor.setSpeed(130);
  setMotorState(false);

  // Static IP configuration
  IPAddress local_IP(192, 168, 1, 200); // Static IP address
  IPAddress gateway(192, 168, 1, 1);    // Router's gateway
  IPAddress subnet(255, 255, 255, 0);   // Subnet mask

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Static IP configuration failed!");
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure web server routes
  server.on("/", handleRoot);
  server.on("/set", HTTP_POST, handleSetTarget);

  // Start the web server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Handle web server requests
  server.handleClient();

  // Periodic humidity check
  if (millis() - lastCheck >= checkInterval) {
    lastCheck = millis();

    // Read current humidity
    float humidity = dht.readHumidity();

    if (isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.println("Current humidity: " + String(humidity) + "%");

    // Control motor based on humidity and target
    if (targetSet && humidity < targetHumidity) {
      setMotorState(true);
    } else {
      setMotorState(false);
    }
  }
}
