#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <SPI.h>;
#include <ESP8266WiFi.h>;
#include <BlynkSimpleEsp8266.h>;
#include <SimpleTimer.h>;
#include <DHT.h>;
//will add flame detection module also

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Dymaoy2x5-SOK5VjFeObLzGjKX6weWLO";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Lord Kim";
char pass[] = "hna12345";
#define pin_switch D2
#define DHTPIN D4 // What digital pin we're connected to
#define ledPin D7
#define pirPin D1
#define buzzer D6

int pirValue;
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11 // DHT 11
#define DHTTYPE DHT22 // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21 // DHT 21, AM2301

boolean oldSwitchState = LOW;
boolean newSwitchState = LOW;

boolean detectstatus = LOW;

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
//int buzzer = D2;
int smokey = A0;

int sensorThres = 1000;

// This function sends Arduino's up time every second to Virtual Pin (5, 6, 7 &amp;amp;amp;amp;amp;amp; 8).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
 float h = dht.readHumidity();
 float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

 if (isnan(h) || isnan(t)) {
 Serial.println("Failed to read from DHT sensor!");
 return;
 }
 // You can send any value at any time.
 // Please don't send more that 10 values per second.
 Blynk.virtualWrite(V2, h); // Humidity for gauge
 Blynk.virtualWrite(V6, t); // Temperature for gauge
 //Blynk.virtualWrite(V2, h); // Humidity for graph
 //Blynk.virtualWrite(V8, t); // Temperature for graph

 if(t > 50){
    Blynk.email("@gmail.com", "House Fire Alert", "Temperature over 50!");
    Blynk.notify("ESP8266 Alert - Temperature over 28C!");
  }
}

void setup()
{
 Serial.begin(115200); // See the connection status in Serial Monitor
 Blynk.begin(auth, ssid, pass);
 pinMode(smokey, INPUT);
pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
 //digitalWrite(ledPin, LOW);
 dht.begin();
pinMode(pin_switch, INPUT);
pinMode(buzzer, OUTPUT);
 // Setup a function to be called every second
 timer.setInterval(1000L, sendSensor);
}
void detection(){
   long state = digitalRead(pirPin);
    if(state == HIGH) {
      digitalWrite (ledPin, HIGH);
      //Serial.println("Motion detected!");
      tone(buzzer, 1000, 10000);
      Blynk.notify("Intruder Alert");
      delay(1000);
    }
    else {
      digitalWrite (ledPin, LOW);
     // Serial.println("Motion absent!");
     noTone(buzzer);
      delay(1000);
      }
  
  }
void loop()
{
 Blynk.run(); // Initiates Blynk
 timer.run(); // Initiates SimpleTimer

 int analogSensor = analogRead(smokey);
 newSwitchState = digitalRead(pin_switch);
 if(newSwitchState != oldSwitchState){
  if(newSwitchState == HIGH){
    if(detectstatus == LOW)
    {detection();}
    else{} 
    
  }
  oldSwitchState = newSwitchState;
  }


 Serial.print("Pin A0: ");
 Serial.println(analogSensor);
  Blynk.virtualWrite(A0,analogSensor);
 // Checks if it has reached the threshold value
 if (analogSensor > sensorThres)
 {
   tone(buzzer, 1000, 10000);
   Blynk.notify("Alert: Fire in the House"); 
   
  // digitalWrite(buzzer, HIGH);
  // delay(200);
   //digitalWrite(buzzer, LOW);
  //delay(200);
    
 }
 else
 {
   noTone(buzzer);
 }
 delay(100);
}
