
#include <DHT.h>

#define DHT_PIN 16
#define DHT_TYPE DHT11

const int LED_RED = 25;
const int LED_YELLOW = 26;
const int LED_GREEN = 27;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  Serial.println("Mini Climate Station Starting...");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();


  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("SENSOR ERROR");

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);

    delay(2000);
    return;
  }


  float heatIndex = dht.computeHeatIndex(
    temperature, humidity, false
  );


  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);


  if (temperature >= 31) {
    digitalWrite(LED_RED, HIGH);
    Serial.println("Status: HOT");
  }
  else if (temperature < 24 ||
           humidity < 40 ||
           humidity > 75) {
    digitalWrite(LED_YELLOW, HIGH);
    Serial.println("Status: CAUTION");
  }
  else {
    digitalWrite(LED_GREEN, HIGH);
    Serial.println("Status: COMFORTABLE");
  }


  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");


  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");


  Serial.print("Heat Index: ");
  Serial.print(heatIndex);
  Serial.println(" C");

  Serial.println("--------------------");

  delay(2000);
}
