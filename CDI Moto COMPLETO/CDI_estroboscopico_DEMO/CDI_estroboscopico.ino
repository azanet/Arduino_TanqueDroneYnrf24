/* 
 * Licencia CC, haz lo que quieras con el código =D.
*/



//#include <Wire.h>
//#include <LiquidCrystal_PCF8574.h>

//#define LCD_Address 0x3F

//LiquidCrystal_PCF8574 lcd(LCD_Address);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int sensorPin = A0;    // select the input pin for the potentiometer
const int ledPin = 13;      // select the pin for the LED
const int bobinaPin = 9;      // select the pin for the BOBINAAAA, GATE del SCR
float outputValue = 0;  // variable to store the value coming from the sensor
int sensorValue = 0;
float freq = 0;

const int Pulsador = A1;  //Agregando Pulsador para control de DELAYS
int PulsadorCount = 0;


void setup()
{
// lcd.begin(16, 2); // initialize the lcd
///lcd.setBacklight(255);
 //lcd.clear();
// lcd.setCursor(0,0);
// lcd.print(" ElectronicMan ");
// lcd.setCursor(0,1);
 //lcd.print("RPM: ");
 pinMode(ledPin, OUTPUT); 
 pinMode(bobinaPin, OUTPUT);
 pinMode(Pulsador, INPUT);
 
 
 Serial.begin(9600);
 
 
 
} // setup()

void loop()
{
  // Lectura del potenciometro y mapping
  sensorValue = analogRead(sensorPin);
  int outputValue = map(sensorValue, 0, 5, 0, 1);


//creando condiciones para realizar los retardos
  
  
  if (digitalRead(Pulsador) != 0){

    if (PulsadorCount >= 8){
      PulsadorCount = 0;
      delay(500);
      }
    else {
      PulsadorCount = PulsadorCount + 1;
      delay(500);
    }
  }







  // Imprimimos por puerto de serie
  //Serial.print("RPM = " );        
  freq = (1000)*((int)outputValue);

  if (outputValue > 0)
  { 
   
    
   /* 
    //Encendemos el led
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH); 
  // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
  // Apagamos el led 
    digitalWrite(ledPin, LOW);
    digitalWrite(bobinaPin, LOW);
    Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue);   
  
  
//   estos DELAYS juntos causan un retardo de una vuelta completa (16mS)

   delay(2);
   delay(2);
   delay(2);
   delay(2);
   delay(2);
   delay(2);
   delay(2);
   delay(2);*/
   if (PulsadorCount == 0){
    
    //retardo de 1ms para atrasar/adelantar la chispa)
    delay(0);

    //Prende la chispa
    digitalWrite(ledPin, HIGH);
    digitalWrite(bobinaPin, HIGH); 
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);
    digitalWrite(bobinaPin, LOW);   
   // Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue);   
   }
   


//PRUEBA RETARDOS CON CONDICIONES

   else if (PulsadorCount == 1){
    
    //retardo de 1ms para atrasar/adelantar la chispa)
    delay(1);

    //Prende la chispa
    digitalWrite(ledPin, HIGH);
    digitalWrite(bobinaPin, HIGH); 
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);   
    digitalWrite(bobinaPin, LOW);   
   // Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue);   
   }
   
   else if (PulsadorCount == 2){

        //retardo de 2ms para atrasar/adelantar la chispa)
    delay(2);

    //Prende la chispa
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH);
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);   
    digitalWrite(bobinaPin, LOW);   
   // Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue); 
   }
   
   else if (PulsadorCount == 3){

        //retardo de 1ms para atrasar/adelantar la chispa)
    delay(3);

    //Prende la chispa
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH);
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);  
    digitalWrite(bobinaPin, LOW);    
    //Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue); 
   }
   
   else if (PulsadorCount == 4){

        //retardo de 1ms para atrasar/adelantar la chispa)
    delay(4);

    //Prende la chispa
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH);
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);   
    digitalWrite(bobinaPin, LOW);   
   // Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue); 
   }
   
   else if (PulsadorCount == 5){

        //retardo de 1ms para atrasar/adelantar la chispa)
    delay(5);

    //Prende la chispa
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH);
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);   
    digitalWrite(bobinaPin, LOW);   
  //  Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue); 
   }
   
   else if (PulsadorCount == 6){

        //retardo de 1ms para atrasar/adelantar la chispa)
    delay(6);

    //Prende la chispa
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH);
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);   
    digitalWrite(bobinaPin, LOW);   
  //  Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue); 
   } 
   
   else if (PulsadorCount == 7){

        //retardo de 1ms para atrasar/adelantar la chispa)
    delay(7);

    //Prende la chispa
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH);
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);   
    digitalWrite(bobinaPin, LOW);   
   // Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue); 
   } 
   
   else if (PulsadorCount == 8){

        //retardo de 1ms para atrasar/adelantar la chispa)
    delay(8);

    //Prende la chispa
    digitalWrite(ledPin, HIGH); 
    digitalWrite(bobinaPin, HIGH);
    // Esperamos 1 ms para que el encendido sea visible
    delay(1);         
    // Apagamos el led 
    digitalWrite(ledPin, LOW);  
    digitalWrite(bobinaPin, LOW);    
    //Serial.println(outputValue);
    Serial.println(60*freq);        
    delay(outputValue); 
   } 
  Serial.println(PulsadorCount);
  Serial.println(outputValue);
  }
  

//  digitalWrite (ledPin, LOW);

 // lcd.setCursor(5,1);
//lcd.print(60*freq);
} // loop()
