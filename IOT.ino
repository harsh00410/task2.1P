#include <WiFiNINA.h>      // For handling the WiFi connection
#include <ThingSpeak.h>    // To communicate with the ThingSpeak platform
#include <DHT.h>           // Required for interfacing with the DHT sensor

// Define the pin where the DHT sensor is connected
#define DHTPIN 7
// Specify the model of the DHT sensor used (DHT11)
#define DHTTYPE DHT11

// Create a DHT object with the defined pin and sensor type
DHT dht(DHTPIN, DHTTYPE);

// WiFi network credentials
#define SECRET_SSID "gopesh's galaxy"
#define SECRET_PASS "gopesh00"

// ThingSpeak channel ID and API key for authentication
#define SECRET_CH_ID 2635438
#define SECRET_WRITE_APIKEY "6039ZG9YHG0SIAGW"

// Store network name (SSID) and password in character arrays
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Create a WiFi client instance
WiFiClient client;

// ThingSpeak channel details
unsigned long channelID = SECRET_CH_ID;
const char* apiKey = SECRET_WRITE_APIKEY;

// The setup function runs once at the start
void setup() {
  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);
  // Wait for the serial port to connect
  while (!Serial) {
    ; // Hold until the serial port is ready
  }

  // Start the DHT sensor
  dht.begin();

  // Begin WiFi connection
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, pass);

  // Wait until the board connects to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Initialize communication with ThingSpeak
  ThingSpeak.begin(client);
}

// The loop function runs repeatedly after setup
void loop() {
  // Pause execution for 2 seconds
  delay(2000);

  // Get humidity and temperature readings from the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Validate the sensor readings
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display the readings in the serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Set the fields in ThingSpeak for humidity and temperature
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);

  // Send the data to ThingSpeak and check if it was successful
  int responseCode = ThingSpeak.writeFields(channelID, apiKey);
  if (responseCode == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.print("Problem updating channel. HTTP error code ");
    Serial.println(responseCode);
  }
}
