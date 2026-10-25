#include <DHT.h>

// Khai báo chân kết nối
#define DHT_PIN 16
#define DHT_TYPE DHT11

#define LED_RED 17
#define LED_YELLOW 18
#define LED_GREEN 19
DHT dht(DHT_PIN, DHT_TYPE);

void turnOffAll(){
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  turnOffAll();

  Serial.println("DHT11 Sensor Starting...");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("SENSOR ERROR");
    turnOffAll();
    delay(2000);
    return;
  }

  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Heat Index: ");
  Serial.print(heatIndex);
  Serial.println(" C");

  if (temperature >= 31.0) {
    // Trạng thái HOT: Nhiệt độ >= 31°C -> LED đỏ sáng
    turnOffAll();
    digitalWrite(LED_RED, HIGH);
    Serial.println("Status: HOT");
  } 
  else if (temperature < 24.0 || humidity < 40.0 || humidity > 75.0) {
    // Trạng thái CAUTION: Nhiệt độ < 24°C hoặc độ ẩm < 40% hoặc độ ẩm > 75% -> LED vàng sáng
    turnOffAll();
    digitalWrite(LED_YELLOW, HIGH);
    Serial.println("Status: CAUTION");
  } 
  else {
    // Trạng thái COMFORTABLE: Các trường hợp còn lại -> LED xanh sáng
    turnOffAll();
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("Status: COMFORTABLE");
  }

  Serial.println("--------------------");

  delay(2000);
}