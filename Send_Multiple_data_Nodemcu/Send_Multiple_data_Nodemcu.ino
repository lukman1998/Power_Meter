#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266mDNS.h>  // For OTA with ESP8266
#include <WiFiUdp.h>  // For OTA
#include <ArduinoOTA.h>  // For OTA

#define AUTH "d92Fc-pGBpYejf2SAANTZ4ypBwV9CUzC"  // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "Galaxy A03s0911"                   //Enter Wifi Name
#define WIFI_PASS "123454321"          //Masukan  wifi Password

#define SERVER "blynk-cloud.com "             // Comment-out if use Blynk hosted cloud service
#define PORT 8442

BlynkTimer timer;

#include <SoftwareSerial.h>

SoftwareSerial Arduino (D1, D2); // RTX TX



char c;
String dataIn;
int8_t indexOfA, indexOfB,indexOfC,indexOfD ;
String data1, data2, data3, data4;
#define OTA_HOSTNAME "Home_Automation"


void myTimer()
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  Blynk.virtualWrite(V0, data1);
  Blynk.virtualWrite(V1, data2);
  Blynk.virtualWrite(V2, data3);
  Blynk.virtualWrite(V3, data4);
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS, "prakitblog.com", 8181); //BLYNK APP PORT 9443
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // For OTA - Use your own device identifying name
  ArduinoOTA.begin();  // For OTA
  //OPEN SERIAL KOMUNIKASI ARDINO-NODEMCU
  Arduino.begin(9600);
  timer.setInterval(1000L, myTimer);

}

void loop() {
  // put your main code here, to run repeatedly:

  while (Arduino.available() > 0)
  {
    c = Arduino.read();



    if (c == '\n')
    {
      break;
    }
    else
    {
      dataIn += c;
    }
  }
  if (c == '\n')
  {
    Parse_data();

    //TAMPILKAN SEMUA DATA
    Serial.println("Voltase  : " + data1 + "V");
    Serial.println("Amper    : " + data2 + "I");
    Serial.println("Power  : " + data3 + "watt");
    Serial.println("KWH    : " + data4 + "kwh");
    Serial.println("+===================+");

    //RESET VARIABEL
    c = 0;
    dataIn = "";
  }
  Blynk.run();
  ArduinoOTA.handle();  // For OTA
  timer.run();
}

void Parse_data() {
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");
  indexOfD = dataIn.indexOf("D");

  data1 = dataIn.substring (0, indexOfA);
  data2 = dataIn.substring (indexOfA + 1, indexOfB);
  data3 = dataIn.substring (indexOfB + 1, indexOfC);
  data4 = dataIn.substring (indexOfC + 1, indexOfD);

}
