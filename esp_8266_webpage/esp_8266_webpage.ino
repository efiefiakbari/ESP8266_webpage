#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "...erfan....";
const char* password = "Efiefiakbari774159633";

ESP8266WebServer server(80);

// LED pin
const int ledPin = 2; // GPIO2 on ESP8266-12E

// Variable to store the LED status (0 for OFF, 1 for ON)
int ledStatus = 0;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set LED pin as OUTPUT
  pinMode(ledPin, OUTPUT);
  
  // Initialize the LED status based on the actual pin state
  ledStatus = digitalRead(ledPin);

  // Handle root URL '/'
  server.on("/", HTTP_GET, [](){
    String html = "<html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; background-color: #f2f2f2; }";
    html += "h1 { color: #333; margin-top: 20px; }";
    html += ".button-container { display: flex; justify-content: center; align-items: center; height: 60vh; }";
    html += ".button { background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer; margin: 10px; border-radius: 5px; font-size: 18px; transition: transform 0.2s, background-color 0.2s; }";
    html += ".button:hover { background-color: #45a049; transform: scale(1.1); }";
    html += ".button:active { background-color: #388e3c; transform: scale(0.9); }";
    html += "@keyframes blink {";
    html += "  0% { opacity: 1; }";
    html += "  50% { opacity: 0.5; }";
    html += "  100% { opacity: 1; }";
    html += "}";
    html += ".blink { animation: blink 1s infinite; }";
    html += "</style>";
    html += "<script>";
    html += "function turnOn() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('GET', '/on', true);";
    html += "  xhr.send();";
    html += "  document.getElementById('led-status').innerHTML = 'LED turned on';";
    html += "  document.getElementById('led-status').classList.add('blink');";
    html += "}";
    html += "function turnOff() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('GET', '/off', true);";
    html += "  xhr.send();";
    html += "  document.getElementById('led-status').innerHTML = 'LED turned off';";
    html += "  document.getElementById('led-status').classList.remove('blink');";
    html += "}";
    html += "window.onload = function() {";
    html += "  document.getElementById('led-status').innerHTML = 'LED " + String(ledStatus ? "turned on" : "turned off") + "';";
    html += "  if (" + String(ledStatus) + ") {";
    html += "    document.getElementById('led-status').classList.add('blink');";
    html += "  }";
    html += "};";
    html += "</script>";
    html += "</head><body>";
    html += "<h1>ESP8266 LED Control</h1>";
    html += "<div class='button-container'>";
    html += "<button class='button' onclick='turnOn()'>Turn On</button>";
    html += "<button class='button' onclick='turnOff()'>Turn Off</button>";
    html += "</div>";
    html += "<p id='led-status'></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  // Handle '/on' URL
  server.on("/on", HTTP_GET, [](){
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED turned on");
  });

  // Handle '/off' URL
  server.on("/off", HTTP_GET, [](){
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED turned off");
  });

  server.begin();
}

void loop() {
  server.handleClient();
  ledStatus = digitalRead(ledPin);

}
