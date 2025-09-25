#include <WiFi.h>
#include <PubSubClient.h>

#define LED_RED_ONE 12
#define LED_YELLOW_ONE 13 
#define LED_GREEN_ONE 14
#define LED_BLUE_ONE 27
#define VIBRATE 26

// --- WIFI & MQTT CONFIGURATION ---
const char* ssid = "TDP Workshop Network";         // <--- CHANGE THIS
const char* password = "techdome2u"; // <--- CHANGE THIS

const char* mqtt_server = "test.mosquitto.org"; // Public MQTT Broker
const int mqtt_port = 1883;
//***************************
const char* mqtt_command_topic = "soundalert/commands"; // This MUST match the Python script
//***************************

// Initialize WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {   
  // initialize inbuilt LED pin as an output.
  Serial.begin(115200);
  pinMode(LED_RED_ONE, OUTPUT);
  pinMode(LED_YELLOW_ONE, OUTPUT); 
  pinMode(LED_BLUE_ONE, OUTPUT);
  pinMode(LED_GREEN_ONE, OUTPUT); 
  pinMode(VIBRATE, OUTPUT);

  Serial.println("Wearable Alert System - Phase 3 Test");

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); // Configure MQTT server
  client.setCallback(callback); // Set the function to run when a message arrives
}


void loop() {
  if (!client.connected()){
    reconnect(); // If not connected to MQTT, try to reconnect
  }
  client.loop(); // This function is crucial, it keeps the MQTT client running
}


// --- WIFI SETUP FUNCTION ---
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// --- MQTT RECONNECT FUNCTION ---
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-"; // Create a random client ID
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe to the command topic once connected
      client.subscribe(mqtt_command_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}

// --- CALLBACK FUNCTION: THIS IS THE HEART OF THE SUBSCRIBER ---
// This function is automatically called by the PubSubClient library
// whenever a message arrives on a topic we are subscribed to.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Convert the payload (message content) to a string
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // --- Command Center ---
  if (message == "FIRE") {
    alert_fire();
  } else if (message == "SIREN") {
    alert_siren();
  } else if (message == "DOG" || message == "ANIMAL" || message == "ANIMAL2" || message == "ANIMAL3") {
    alert_animal();
  } else if (message == "SILENCE"){
    alert_silence();
  } else if (message == "SPEECH"){
    alert_speech(); 
  } else if (message == "OFF") {
    all_off();
  }
}


void alert_fire(){
  for (int i = 0; i<5 ; i++){
    digitalWrite(LED_RED_ONE, HIGH);
    delay(250);
    digitalWrite(LED_RED_ONE, LOW);
    delay(250); 
    digitalWrite(VIBRATE, HIGH);
    delay(250); 
    digitalWrite(VIBRATE, LOW);
    delay(250); 
  }
}

void alert_siren(){
  for (int i = 0; i<3 ; i++){
    digitalWrite(LED_YELLOW_ONE, HIGH);
    delay(250);
    digitalWrite(LED_YELLOW_ONE, LOW);
    delay(250); 
  }
}

void alert_animal(){
  for (int i = 0; i<5 ; i++){
    digitalWrite(LED_YELLOW_ONE, HIGH);
    digitalWrite(LED_RED_ONE, HIGH);  
    digitalWrite(LED_BLUE_ONE, HIGH);
    digitalWrite(LED_GREEN_ONE, HIGH); 
    digitalWrite(VIBRATE, HIGH);  
    delay(100);
    digitalWrite(LED_YELLOW_ONE, LOW);
    digitalWrite(LED_RED_ONE, LOW);
    digitalWrite(LED_BLUE_ONE, LOW);
    digitalWrite(LED_GREEN_ONE, LOW);
    digitalWrite(VIBRATE, LOW);
    delay(100);
  }
}

void alert_vehicle(){ 
    digitalWrite(LED_BLUE_ONE, HIGH);
    digitalWrite(LED_GREEN_ONE, HIGH); 
    digitalWrite(VIBRATE, HIGH); 
    delay(150);
    digitalWrite(LED_BLUE_ONE, LOW);
    digitalWrite(LED_GREEN_ONE, LOW);
    digitalWrite(VIBRATE, LOW);
    delay(150);
}

void alert_silence(){
  digitalWrite(LED_YELLOW_ONE, HIGH);
  digitalWrite(LED_RED_ONE, HIGH);    
  delay(2000);
}

void alert_speech(){
  for (int i = 0; i<5 ; i++){
    digitalWrite(VIBRATE, HIGH);
    delay(200);
    digitalWrite(VIBRATE, LOW);
    delay(200); 
  }
}

void all_off(){
  digitalWrite(LED_RED_ONE, LOW);
  digitalWrite(LED_YELLOW_ONE, LOW);
}

