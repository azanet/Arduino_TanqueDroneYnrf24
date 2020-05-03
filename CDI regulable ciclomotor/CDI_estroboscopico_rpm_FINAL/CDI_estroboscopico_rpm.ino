/*
*
*/

const int sensorPin = A0;    //SENSOR HALL
const int ledPin = 13;      // LED INCRUSTADO (ESTROBOSCOPICO)
const int bobinaPin = 9;      // GATE del MOSFET N
int outputValue = 0;  // VARIABBLE PARA GUARDAR VALOR DEL SENSOR
int sensorValue = 0;
int freq = 0;
const int Pulsador = A1;  //Agregando Pulsador para control de DELAYS
int PulsadorCount = 0;

unsigned long time = millis();                    // variable de tiempo
int RPM = 0;                                              // variable para guardar las revoluciones por minuto
int Limpiart1 = 0;                                       // variable para contar una vuelta completa del eje
long t1 = millis();                                     // variable para medir el tiempo al primer paso del eje por el sensor
long t2 = millis();                                     // variable para medir el tiempo al segundo paso del eje por el sensor



void setup()
{

 pinMode(ledPin, OUTPUT); 
 pinMode(bobinaPin, OUTPUT);
 pinMode(Pulsador, INPUT);
 
 Serial.begin(9600);
 
 while (millis() < 5000) {}                       // Espera calibración durante 5 segundos
 time = millis();     
 
 
} // setup()

void loop()
{
  // Lectura del potenciometro y mapping
  sensorValue = analogRead(sensorPin);
  int outputValue = map(sensorValue, 0, 1024, 0, 255);


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
   
   
    if (Limpiart1 == 1) {                              // si es la segunda vez que el eje pasa por el sensor, entonces:
         t2 = millis();                                        // tomar en t2 el valor del reloj interno en milisegundos,
         RPM = 60000 / (t2 - t1);                     // calcular las revoluciones por minuto
         t1 = t2;                                                 // dejar en t1 el valor del reloj tomado antes en t2 para iniciar otra cuenta
         Limpiart1 = 0;                                      // poner a cero el contador de pasos del eje por el sensor
          }       // fin del if (HallState == LOW, o sea imán detectado
    
    else {                                                  // si el estado del sensor no ha detectado el imán, o sea el valor es HIGH, entonces:
         digitalWrite(ledPin, LOW);            // apagar el LED:
          Limpiart1 = 1;                                // poner el contador de paso del imán a 1, es decir, empieza el segundo giro del eje
      }

 
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
   // Serial.println(60*freq);        
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
  Serial.println(RPM);
  }
  

//  digitalWrite (ledPin, LOW);

 // lcd.setCursor(5,1);
//lcd.print(60*freq);
} // loop()
