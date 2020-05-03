/*    PROYECTO INVERNADEROO!! v0.0.1

*/
//LIBRERIAS PANTALLA 5110, DHT Y RELOJ
#include <SPI.h>               // include SPI library
#include <Adafruit_GFX.h>      // include adafruit graphics library
#include <Adafruit_PCD8544.h>  // include adafruit PCD8544 (Nokia 5110) library

#include <DHT.h>               // include DHT library code

#include <Wire.h>
#include "RTClib.h"

// Nokia 5110 LCD module connections (CLK, DIN, D/C, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
RTC_DS3231 rtc;

//DEFINIENDO SENSOR DHT
#define DHTPIN    A3         // DHT22 data pin is connected to Arduino analog pin 0
#define DHTTYPE   DHT22      // DHT22 sensor is used
DHT dht22(DHTPIN, DHTTYPE);  // configure DHT library
char temperature[] = "000.0";
char humidity[]    = "000.0%";

//DEFINIENDO SENSOR HUMEDAD SUELO
#define sensor A2
#define sensor1 A1


//DEFINIENDO BUZZER!!
const int buzzer = 13;


void setup()
{
  delay(1000);  // wait 1 second
  
// INICIALIZANDO PANTALLA NOKIA 5110
  display.begin();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE); 
// you can change the contrast around to adapt the display
// for the best viewing!
  display.setContrast(50);
  display.clearDisplay();   // clear the screen and buffer
  display.display(); 
   
// INICIALIZANDO SENSOR DHT library
  dht22.begin();  
  
   
//INICIALIZANDO MODULO DE RELOOJ DS3231
   if (! rtc.begin()) {
    display.setCursor(0, 0);
    display.print("Couldn't find RTC");
    display.display();
    delay(1000);
  while (1);
  }

  if (rtc.lostPower()) {
    display.setCursor(0, 0);
    display.print("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2019, 6, 30, 21, 42, 0));
    display.display();
    delay(1000);
  } 

  
 
  display.clearDisplay();   // clear the screen and buffer
  display.display();
 
 // display.drawFastHLine(0, 23, display.width(), BLACK);
 
/*
  display.setCursor(0, 0);
  display.print("Tmp:");
  display.setCursor(0, 8);
  display.print("Hg:");
  display.setCursor(0, 16);
  display.print("FL1:");
  display.display();
  display.setCursor(0, 24);
  display.print("FL2:");
  display.display();
*/  
  pinMode(sensor, INPUT);
  pinMode(sensor1, INPUT);
  pinMode(buzzer, OUTPUT);  //definir pin como salida


}
 
// main loop
void loop()
{
  
  //LECTURA HUMEDAD SUELO
  int valorHumedad = map(analogRead(sensor), 0, 1023, 100, 0);
  String bufsuelo= (String) valorHumedad + "%  ";
  
  //LECTURA HUMEDAD SUELO1
  int valorHumedad1 = map(analogRead(sensor1), 0, 1023, 100, 0);
  String bufsuelo1= (String) valorHumedad1 + "%  ";
 

  
  // read humidity
  int RH = dht22.readHumidity() * 10;
  // read temperature in degree Celsius
  int Temp = dht22.readTemperature() * 10;
 
  // update temperature and humidity arrays
  if(Temp < 0)
  {
    temperature[0] = '-';
    Temp = abs(Temp);
  }
  else
    temperature[0] = ' ';
  temperature[1] = (Temp / 100) % 10  + '0';
  temperature[2] = (Temp / 10)  % 10  + '0';
  temperature[4] =  Temp % 10 + '0';
 
  if(RH >= 1000)
    humidity[0] = '1';
  else
    humidity[0] = ' ';
  humidity[1]   = (RH / 100) % 10 + '0';
  humidity[2]   = (RH / 10)  % 10 + '0';
  humidity[4]   =  RH % 10 + '0';

 
  // print temperature
  display.setCursor(15,0);
  display.print(temperature);
  
  // print degree symbol ( ° )
  display.drawRect(46, 0, 3, 3, BLACK);
  // print humidity
  display.setCursor(15, 8);
  display.print(humidity);
 

  
//SENSOR SUELO DATOS
  display.setCursor(25, 16);
  if (valorHumedad <= 9)
  {
    display.print('0');
    display.print(bufsuelo);
  }
  else
    display.print(bufsuelo);
  

//SENSOR SUELO 1 DATOS
  display.setCursor(25, 24);
  if (valorHumedad1 <= 9)
  {
    display.print('0');
    display.print(bufsuelo1);
  }
  else
    display.print(bufsuelo1);

 
  
  
  
//PITIDO DEL BUZZER!!!
  tone(13,293.66,200);
  

    //imprimir HORA DEL RELOOJ!!
 DateTime now = rtc.now();
 display.setCursor(0, 41);
 if (now.hour()< 10)
  {
    display.print ("0");
    display.print (now.hour(), DEC);
  }
 else   
   display.print(now.hour(), DEC);
 display.print(':');
 if (now.minute()< 10)
  {
    display.print ("0");
    display.print(0 + now.minute(), DEC);
  }
 else
  display.print(now.minute(), DEC);
 display.print(':');
 if (now.second()< 10)
  {
    display.print ("0");
    display.print(0 + now.second(), DEC);
  }
 else
  display.print(now.second(), DEC);


// now update the display
 display.display();
 delay(1000);
 //display.display();
}
// end of code.
