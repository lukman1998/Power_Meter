
#include "ACS712.h" // LIBRARAY SENSOR ARUS ACS

#include <Wire.h> // LIBRARAY I2C
#include <LiquidCrystal_I2C.h> // LIBRARY LCD 1602 I2C

//UNTUK SENSOR VOLTAGE ZMPT
#define SAMPLING 300
#define VOFFSET  512
#define AMPLITUDE 720.00
#define REAL_VAC  220.0
int adc_max, adc_min;
int adc_vpp;

const byte ADC_PIN = A0; // PIN ANALAOG UNTUK SENSOR VOLTAGE
const byte ADC_Arus = A1; // PIN ANALAOG UNTUK SENSOR ARUS

LiquidCrystal_I2C lcd(0x27, 16, 2);
// dapat dicek menggunakan i2c finder

ACS712 sensor(ACS712_05B , ADC_Arus);
/* kode untuk penggunaan sensor acs712
   5A : ACS712_05B
   20A : ACS712_20A
   30A : ACS712_30A
*/

#include <SoftwareSerial.h>

SoftwareSerial Nodemcu (10, 11); // RTX TX

int data1, data2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // BAUDRATE SERIAL MONITOR
  Nodemcu.begin(9600); // BAUDRATE KOMUNIKASI SOFTWARE SERIAL
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("ENERGY");
  lcd.setCursor(2,1);
  lcd.print("**METER**");

  //Kalibrasi sensor dengan syarat tidak boleh ada beban terlebih dahulu
  Serial.println("Kalibrasi ... Pastikan tidak ada arus yang mengalir melalui sensor pada saat ini");
  sensor.calibrate();
  Serial.println("Selesai!");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis;
  unsigned long seconds;
  unsigned long minutes;
  unsigned long hours;
  
  currentMillis = millis();
  seconds = currentMillis / 1000;
  minutes = seconds / 60;
  hours = minutes / 60;

  float I = sensor.getCurrentAC(); // FUNGSI BACA ARUS
  read_VAC(); // FUNGSI UNTUK BACA VOLTASE
  float V = map(adc_vpp, 0, AMPLITUDE, 0, REAL_VAC * 100.00) / 100.00; // MAPPING NILAI ADC
  //RUMUS DAYA
  float P = V * I;
  //RUMUS ENERGI
  float kwh = (hours * P)/1000; 
  if (V <= 210)
  {
    V = 212;
  }
  else if (V >= 212 && V <= 220 )
  {
    V = V;
  }
  else if ( V >= 225)
  {
    V = 220;
  }
  Serial.print(String("voltase : ") + V + "V");
  Serial.print("    ");
  Serial.println(String("I = ") + I + " A");
  Serial.print(String("Power : ") + P + "watt");
  Serial.print("    ");
  Serial.println(String("K WH = ") + kwh + " khw");
  Serial.println(seconds);
  Serial.println(minutes);
  Serial.println(hours);
  lcd.setCursor(0,0); lcd.print("V:"); 
  lcd.setCursor(2,0); lcd.print(V); 
  lcd.setCursor(7,0); lcd.print("v"); 
  lcd.setCursor(8,0); 
  lcd.print(" ");
  lcd.setCursor(9,0); lcd.print("I:"); lcd.setCursor(11,0); lcd.print(I);lcd.setCursor(15,0); lcd.print("A");
  lcd.setCursor(2,1); lcd.print(" ");
  lcd.setCursor(3,1); lcd.print("P:"); lcd.setCursor(5,1); lcd.print(P); lcd.setCursor(9,1); lcd.print("watt");


  // KIRIM SOFTWARE SERIAL KE NODEMCU
  Nodemcu.print(V); Nodemcu.print("A");
  Nodemcu.print(I); Nodemcu.print("B");
  Nodemcu.print(P); Nodemcu.print("C");
  Nodemcu.print(kwh); Nodemcu.print("D");
  Nodemcu.print("\n");

  //delay(500);
}


// FUNGSI UNTUK BACA VOLTASE
void read_VAC()
{
  int cnt;
  adc_max = 0;
  adc_min = 1024;

  for (cnt = 0; cnt < SAMPLING; cnt++)
  {
    int adc = analogRead(ADC_PIN);
    //Serial.print(String("ADC : ") + adc + " ");
    if (adc > adc_max)
    {
      adc_max = adc;
    }
    if (adc < adc_min)
    {
      adc_min = adc;
    }
  }
  adc_vpp = adc_max - adc_min;
}
