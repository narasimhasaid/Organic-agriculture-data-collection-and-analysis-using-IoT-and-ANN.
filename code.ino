#include <SoftwareSerial.h>
#include <stdlib.h>
#include <dht.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
int sensorPin = A0;
int sensorValue = 0;
int percentValue = 0;

dht DHT;
#define DHT11_PIN 7

// replace with your channel's thingspeak API
String apiKey = "M0ISV3XJ8SV582PF";
// connect 9 to TX of Serial USB
// connect 10 to RX of serial USB
SoftwareSerial ser(10,9); // RX, TX
// this runs once
void setup()
{

// enable debug serial
Serial.begin(115200);
// enable software serial
// black wifi module the baud rate is 115200
ser.begin(115200);
// reset ESP8266
ser.println("AT+RST");
lcd.begin(16,2);
lcd.clear();
}

void loop()
{
// convert to temp:

// READ DATA from DHT11
int chk = DHT.read11(DHT11_PIN);

lcd.setCursor(0,0);

lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  Serial.print("temp;");
Serial.print(DHT.temperature);

lcd.setCursor(0,1);
lcd.print("Rel_Hum=");
lcd.print(DHT.humidity);
lcd.print("%"); /* Its prints value in PERCENTAGES */
Serial.print("Rel_Hum=");
Serial.print(DHT.humidity);
Serial.println("%"); 
  sensorValue = analogRead(sensorPin);
  Serial.print("\n\nAnalog Value: ");
  Serial.print(sensorValue);
  
  percentValue = map(sensorValue, 1023, 200, 0, 100);
  Serial.print("\nPercentValue: ");
  Serial.print(percentValue);
  Serial.print("%");
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture");
  
  lcd.setCursor(0, 1);  
  lcd.print("Percent: ");
  lcd.print(percentValue);
  lcd.print("%");
  delay(3000);
  lcd.clear();



// convert to string
char buf[16];
String strTemp = dtostrf(DHT.temperature, 4, 1, buf);
char buff[16];
String strHum = dtostrf(DHT.humidity,4,1,buff);
char buffe[18];
String strMois = dtostrf(percentValue,4,1,buffe);
// Internet connection
String cmd = "AT+CIPSTART=\"TCP\",\"";
cmd += "184.106.153.149"; // api.thingspeak.com
cmd += "\",80";
ser.println(cmd);
if(ser.find("Error"))
{
Serial.println("AT+CIPSTART error");
return;
}
String getStr = "GET /update?api_key="; /prepare GET string/
getStr += apiKey;
getStr +="&field1=";
getStr += String(strTemp);
getStr +="&field2=";
getStr +=String(strHum);
getStr +="&field3=";
getStr +=String(strMois);
getStr += "\r\n\r\n"; /* send data length */
cmd = "AT+CIPSEND=";
cmd += String(getStr.length());
ser.println(cmd);
if(ser.find(">"))
{
ser.print(getStr);
}
else
{
ser.println("AT+CIPCLOSE"); /* alert user */
Serial.println("AT+CIPCLOSE");
}
delay(5000); /thingspeak needs 16 sec delay between updates/
}