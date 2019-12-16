//RECEPTOR NRF24L01
//ARDUINO MEGA

//Agregando librerias
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//configurando RF 2,4Ghz
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F4F0E1LL;

int data[1];
int control=0;
int lastcontrol=0;



//MIENTRAS "ESTADO ES 0" LA MAQUINA ESTARA EN MODO AUTO Y EL LED ESTARA PRENDIDO, SI SE VUELVE 1 (MANUAL), EL LED SE APAGARA
int estado = 1;
//int ledestado = A0;

//COLOCAMOS TODAS LAS ENTRADAS Y SALIDAS DE SENSORES Y SUS NUMEROS DONDE LOS CONECTAMOS
//ULTRASONIDO-LED DERECHA

//int sigled2 = A2; //led prueba rojo- DERECHA
int trigPin2 = 30;    //Trig - ROJO
int echoPin2 = 31;    //Echo - BLANCO

//ULTRASONIDO-LED IZQUIERDA

//int sigled = 13; //led prueba ROJO-IZQUIERDA
int trigPin1 = 28;    //Trig - ROJO
int echoPin1 = 29;    //Echo - BLANCO

//VARIABLES PARA DETERMINAR LA DISTANCIA, AGREGAR 3 MAS PARA CADA SENSOR
long duration, cm, inches, duracion, espacio, nose;

//MOTORES
//Motor A
int enableA = 22;//CONTROL-A 
int motorA1 = 23;//BOBINA-A1
int motorA2 = 24;//BOBINA-A2

//Motor B
int enableB = 25;//CONTROL-B 
int motorB1 = 26;//BOBINA-B1
int motorB2 = 27;//BOBINA-B2

//variables para REREACCIONES!!!
int rereac = 0;
long reduration, recm, reinches, reduracion, reespacio, renose;
int giroreac = 0;

void setup() {
    //Serial Port begin y RF 2,4Ghz
    radio.begin();
    Serial.begin(9600);
    radio.openReadingPipe(1, pipe);
    radio.startListening();
    radio.setPALevel(RF24_PA_HIGH); 
    radio.setDataRate(RF24_250KBPS);

    //Definimos LAS QUE SON entradas O salidas
    
    //ECHO-IZQUIERDA
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
     //  pinMode(sigled, OUTPUT);
    
    //ECHO-DERECHA
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    //pinMode(sigled2, OUTPUT);
    
  //MOTOR-DERECHA
    pinMode (enableA, OUTPUT); 
    pinMode (motorA1, OUTPUT);
    pinMode (motorA2, OUTPUT);
  
  //MOTOR-IZQUIERDA
    pinMode (enableB, OUTPUT); 
    pinMode (motorB1, OUTPUT);
    pinMode (motorB2, OUTPUT);

}

/*
  ////////////////////////////////////
  //// configuramos acciones de motor para simplificar el codigo
  ///////////////////////////////////
*/

//ACTIVAMOS MOTORES A(motor derecho) y B(motor izquierdo)
void motorON () {

    digitalWrite (enableA, HIGH);
    digitalWrite (enableB, HIGH);

}


//solo la rueda derecha gira hacia ATRAS
void IzqAtras () {
    digitalWrite (motorB1, LOW); 
    digitalWrite (motorB2, LOW);
    digitalWrite (motorA1, LOW); 
    digitalWrite (motorA2, HIGH);
}

//solo la rueda derecha gira hacia ALANTE
void IzqAlante () {
    digitalWrite (motorB1, LOW); //
    digitalWrite (motorB2, LOW);
    digitalWrite (motorA1, HIGH); //
    digitalWrite (motorA2, LOW);
}

//solo la rueda izquierda gira hacia ATRAS
void DchaAtras () {
    digitalWrite (motorA1, LOW); //
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, LOW); //
    digitalWrite (motorB2, HIGH);
}

//solo la rueda izquierda gira hacia ALANTE
void DchaAlante () {
    digitalWrite (motorA1, LOW); //
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, HIGH); //
    digitalWrite (motorB2, LOW);
}

//ATRAS
void atras () {
    digitalWrite (motorA1, LOW);
    digitalWrite (motorA2, HIGH);
    digitalWrite (motorB1, LOW);
    digitalWrite (motorB2, HIGH);
    //Durante 3 segundos

}

//ALANTE

void alante () {
    digitalWrite (motorA1, HIGH);
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, HIGH);
    digitalWrite (motorB2, LOW);
  //Durante 3 segundos

}
//GIRAR izquierda360

void izquierda360 () {
    digitalWrite (motorA1, HIGH);
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, LOW);
    digitalWrite (motorB2, HIGH);
  //Durante 3 segundos

}
// GIRAR derecha360
void derecha360 () {
    digitalWrite (motorA1, LOW); 
    digitalWrite (motorA2, HIGH);
    digitalWrite (motorB1, HIGH); 
    digitalWrite (motorB2, LOW);
  //Durante 3 segundos
}

//PARAR MOTORES
void motorOFF () {
        digitalWrite (enableA, LOW);
        digitalWrite (enableB, LOW);
        digitalWrite (motorA1, LOW);
        digitalWrite (motorA2, LOW);
        digitalWrite (motorB1, LOW);
        digitalWrite (motorB2, LOW);
        
}//Fin motorOFF

//ULTRASONIDOS

void ultizq (){
    //SENSOR hc-sr04 IZQUIERDA ultrasonido
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    pinMode(echoPin1, INPUT);
    duration = pulseIn(echoPin1, HIGH);
    // convert the time into a distance
    cm = (duration / 2) / 27.1;
    inches = (duration / 2) / 74;
  }

void ultdcha (){
   //SENSOR hc-sr04 DERECHA
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    pinMode(echoPin2, INPUT);
    duracion = pulseIn(echoPin2, HIGH);
    // convert the time into a distance
    espacio = (duracion / 2) / 27.1;
    nose = (duracion / 2) / 74;
}



//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void loop()
{   

    /////////////////////////////////////////////
  //////////////////////////////////////////////
  ///PRIMERA PARTE, PERTENECE A RC//////////////
  ////////////////////////////////////////////////////
  /////CONFIGURACION DE ACCIONES RECIBIDAS POR RC//////
  ////////////////////////////////////////////////////


    
    if (radio.available()){
     
        radio.read(data, sizeof(data));
        Serial.println(data[0]);
        control = data[0]; 
    }//FIN IF (radio.available)

    
    if (control==0 && lastcontrol==2){
        lastcontrol=control;
        }
          
    if (lastcontrol==2){
        control=0;
      }
    
      
    if (estado == 1) {
        //SE APAGA EL LED VERDE, SE INICIA MODO MANUAL
        //digitalWrite (ledestado, LOW);

      
        /////////////////////////////////////////////////////
        ////ACTUANDO SEGUN DATOs RECIBIDOs DEL MANDO RC////////
        ////////////////////////////////////////////////////

        //apagar el modo manual y activar modo automatico
        if (control == 2 && lastcontrol==0){
            
            estado = 0;
            lastcontrol=control;
            motorOFF ();
      

        } else if (control == 5) {
            //ALANTE 
            motorON ();
            alante ();
           

        } else if (control == 3) {

           //HAY QUE CREAR LA FUNCION PARA ACTIVAR UNA LUZ


        } else if (control == 6) {
            //ATRAS
            motorON ();
            atras ();
          

        } else if (control == 8) {
            motorON ();
            //girar en 360 hacia derecha
            derecha360 ();
  

        } else if (control == 7) {
            motorON ();
            //girar en 360 hacia izquierda
            izquierda360 ();
           

        } else if (control == 11) {
            motorON ();
            //solo la rueda  derecha gira hacia atras
            IzqAtras ();
            

        } else if (control == 12) {
            motorON ();
            //solo la rueda izquierda gira hacia atras
            DchaAtras ();
            

        } else if (control == 10) {
            motorON();
            //solo la rueda izquierda gira hacia delante a velocidad1
            DchaAlante();
            

        } else if (control == 4) {
        
            //HAY QUE CREAR LA FUMCION, PARA LA BOCINAAAA!!!
            

        } else if (control == 9) {
            motorON();
            //solo la rueda derecha gira hacia delante a velocidad1
            IzqAlante();
            
        
        
        }else{
            //paramos motores
            motorOFF ();    
     
        }//Fin ELSE-IF que compara que se está recibiendo

     
//=======final del "si estado es 1"=================================================================================
    
    }else { //SI "ESTADO" ES 0, COMENZARa EL MODO AUTOMATICO.

    //SE ENCENDERA EL LED VERDE EN MODO AUTOMATICO
    //    digitalWrite (ledestado, HIGH);


    //////////////////////////////////////////////
    ////////////MODO////AUTOMATICO////////////////
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    //------------------------------------------

    //VARIABLE PARA ACCION DE MOTOR Y SENSORES, IRA DE 0 A 4, TENDRA 4 ACCIONES Y UN ESTADO INICIAL SIEMPRE VUELVE A 0 AL FINAL DEL LOOP
   
    int reaccion = 0;

    //SENSOR Y LED IZQUIERDA
    ultizq ();
    Serial.print(cm);
    //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
    int cm1 = cm;


    if (cm1 < 25) {
      //      digitalWrite (sigled, HIGH);
      reaccion = 1;
    }
    else
    {
      //      digitalWrite (sigled, LOW);
      reaccion = 6;
    }
    //--------------------------------

    //SENSOR Y LED DERECHA
    
    ultdcha ();
    Serial.print(espacio);
    //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
    int cm2 = espacio;

    delay(0);

    if (cm2 < 25) {
      //      digitalWrite (sigled2, HIGH);
      reaccion = reaccion + 3;
    }
    else
    {
      //      digitalWrite (sigled2, LOW);
      reaccion = reaccion + 4;
    }
    Serial.print(reaccion);

    //QUITAR DELAY PARA QUE VAYA RAPIDO, ESTA PUESTO PARA MIRAR LOS RESULTADOS EN LA PANTLLA
    delay(0);

    //-----------------------------------
    /*AQUE DECLARAREMOS QUE HARA EL ROBOT, SEGUN EL VALOR DE LA VARIABLE 'reaccion'
       4: LOS DOS SENSORES DETECTAN ALERTA.
       5:SENSOR IZQUIERDA DETECTA OBTACULO FRONTAL
       9:SENSOR DERECHA DETECTA OBTACULO FRONTAL
       10:AMBOS SENSORES NO DETECTAN ALERTAS
    */

    //4: LOS DOS SENSORES DETECTAN ALERTA.
    if (reaccion == 4) {
      
      motorON ();
      Serial.println ("Hacia atras");
      atras ();
      delay (250);
      
      //EL ROBOT GIRARÃ� HACIA DONDE HALLA MAS SITIO LIBRE COMPARANDO LA DISTANCIA MEDIDA ENTRE LOS DOS SENSORES
      if (cm1 <= cm2) {

        derecha360 ();
        delay (250);

      } else {
        Serial.println ("Hacia IZQUIERDA");
        izquierda360 ();
        delay (250);
      }
      /////////////////////////////////////////////////////////////////////
      //1111111111....11111111111111111--------CREANDO  REREACCIONEEESSS!!//
      //////////////////////////////////////////////////////////////////////
      //Colocamos las bobinas en estado de apagado y LANZAMOS PINGS


      //LANZANDO PING DE DENTRO DE LA ALERTA-4, PARA USARLOS EN LAS MANIOBRAS Y HAGAN DE 'MEMORIA'

      //SENSOR Y LED IZQUIERDA
      ultizq();
      Serial.print(cm);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      cm1 = cm;
      Serial.print("cm");
      Serial.println();
      

      if (cm1 < 20) {
        //        digitalWrite (sigled, HIGH);
        rereac = 1;
      
      }
      else
      {
        //        digitalWrite (sigled, LOW);
        rereac = 6;
       
      }
      //---------sensor derecha---------------
  
      //SENSOR Y LED DERECHA
      ultdcha ();

      Serial.print(espacio);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      int cm2 = espacio;
      Serial.print("cm");
      Serial.println();
     

      if (cm2 < 20) {
        //        digitalWrite (sigled2, HIGH);
        rereac = rereac + 3;
   
      }
      else
      {
        //        digitalWrite (sigled2, LOW);
        rereac = rereac + 4;
           }
      Serial.print(rereac);
      Serial.print("rereac");
   
      ////////////////////////////////////
      //CONFIGURACION DE REREACCIONES ////
      /////
      ////////////////////////////////////

      //LOS DOS DETECTAN PRESENCIA rereacc=4///
      
      motorON ();
      
      if (rereac == 4) {
        //EL ROBOT GIRARÃ� HACIA DONDE HALLA MAS SITIO LIBRE COMPARANDO LA DISTANCIA MEDIDA ENTRE LOS DOS SENSORES
        if (cm1 <= cm2) {

          derecha360 ();
          //TIEMPO DE GIRO
          delay (250);

        } else {

          Serial.println ("Hacia IZQUIERDA");
          izquierda360 ();
          //TIEMPO DE GIRO
          delay (250);
        }

        //SENSOR IZQUIERDA-- SI DETECTA A LA IZQ, GIRARA A LA DERECHA
      } else if (rereac == 5) {
        derecha360 ();
        //Durante 3 segundos
        delay (250);

        //SENSOR DERECHA-- SI DETECTA A LA DERECHA GIRARA A IZQ.
      } else if (rereac == 9) {
        Serial.println ("Hacia IZQUIERDA");
        izquierda360 ();
        //Durante 3 segundos
        delay (250);

        //NO DETECTAN NADA
      } else if (rereac == 10) {
        //PARAR MOTORES

      }

      //FIN DE REREACCIONES DE LA ALERTA-4///////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////
      //===================================================================================
      ///////////////////////////////////////////////////////////////////////
      ////22222--REREREACCIONES DE LA ALERTA 4, SI VUELVEN A DETECTAR LOS SENSORES DESPUES DE LAS PRIMERAS REREACCIONES
      ///////////////////////////////////////////////////////////////////////////////
      //SENSOR Y LED IZQUIERDA
      ultizq ();

      Serial.print(cm);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      cm1 = cm ;
      Serial.print("cm");
      Serial.println();
      

      if (cm1 < 20) {
        //        digitalWrite (sigled, HIGH);
        rereac = rereac + 13;
      
      }
      else
      {
        //        digitalWrite (sigled, LOW);
        rereac = rereac + 16;
       
      }
      //--------------------------------
     
      //SENSOR Y LED DERECHA
      ultdcha ();
      Serial.print(espacio);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      cm2 = espacio;
      Serial.print("cm");
      Serial.println();
      

      if (cm2 < 20) {
        //        digitalWrite (sigled2, HIGH);
        rereac = rereac + 20;
        
      }
      else
      {
        //        digitalWrite (sigled2, LOW);
        rereac = rereac + 44;
       
      }
      Serial.print(rereac);
      Serial.print("rereac");
    
      int rerereac = rereac;

      /*
        37=   ambos dtectan + ambos detectan   =   gira 1 segundo
        40=   ambos dtectan + derecha          =  gira derecha
        61=   ambos dtectan + izquierda        = gira izquierda
        38=   izquierda     + ambos detectan   =  compara cm1 y cm2 y gira
        41=   izquierda     +  derecha         =  compara y gira  hacia donde hay menos espacio
        62=   izquierda     +   izquierda      =  gira izquierda
        42=   derecha       +  ambos detectan  = compara cm1 y cm2 y gira
        45=   derecha       +     derecha      = gira derecha
        66=   derecha       +    izquierda     = compara y gira  hacia donde hay menos espacio
      */

      //////////////////////////////////////
      //CONFIGURACION DE REREREACCIONES ////
      /////
      ////////////////////////////////////

      Serial.println ("Activamos motores");
      motorON ();

      //37=  va hacia atras
      if (rerereac == 37) {

        if (cm1 <= cm2) {

          derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
          izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }

        //40=  gira derecha
      } else if (rerereac == 40) {
        derecha360 ();
        //Durante 3 segundos
        delay (350);
        
        //61= gira izquierda
      } else if (rerereac == 61) {
        izquierda360 ();
        //Durante 3 segundos
        delay (350);
       
        //38=  compara cm1 y cm2 y gira
      } else if (rerereac == 38) {

        if (cm1 <= cm2) {

          derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
          izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }
       
        //41=  si
      } else if (rerereac == 41) {

        if (cm1 >= cm2) {

         derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
         izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }
        //62=  gira izquierda
      } else if (rerereac == 62) {
        Serial.println ("Hacia IZQUIERDA");
        izquierda360 ();
        delay (350);
      
        //42= compara cm1 y cm2 y gira
      } else if (rerereac == 42) {

        if (cm1 <= cm2) {

          derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
         izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }
      
        //45= gira derecha
      } else if (rerereac == 45) {
        derecha360 ();
        //Durante 3 segundos
        delay (350);
       
        //66 = compara y decide solo donde girar
      } else if (rerereac == 66) {

        if (cm1 >= cm2) {

          derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
          izquierda360 ();
          delay (350);
        }
      }

      /////////////////////////////////////////////////////////////////////////////////
      //FIN DE REREREACCIONES ALERTA-4///////////////
      //===================================================================================================

      //5:SENSOR izquierda DETECTA OBTACULO
    } else if (reaccion == 5) {
      //ATRAS
      Serial.println ("Activamos motores");
      motorON ();
      
      Serial.println ("Hacia atrÃ¡s");
      atras ();
      //Durante 3 segundos
      delay (200);

      // GIRAR DERECHA
      derecha360 ();
      //Durante 3 segundos
      delay (250);
  

      //LANZANDO PING DE DENTRO DE los giros, PARA USARLOS EN LAS MANIOBRAS Y HAGAN DE 'MEMORIA'

      //SENSOR Y LED IZQUIERDA
      ultizq ();

      Serial.print(cm);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      cm1 = cm;
      Serial.print("cm");
      Serial.println();
      

      if (cm1 < 25) {
        //        digitalWrite (sigled, HIGH);
        giroreac = 1;
      
      }
      else
      {
        //        digitalWrite (sigled, LOW);
        giroreac = 6;
      
      }
      //---------sensor derecha---------------
      
      //SENSOR Y LED DERECHA
      ultdcha ();

      Serial.print(espacio);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      int cm2 = espacio;
      Serial.print("cm");
      Serial.println();
    

      if (cm2 < 25) {
        //        digitalWrite (sigled2, HIGH);
        giroreac = giroreac + 3;
    
      }
      else
      {
        //        digitalWrite (sigled2, LOW);
        giroreac = giroreac + 4;
   
      }
      Serial.print(rereac);
      Serial.print("rereac");
   

      ////////////////////////////////////
      //CONFIGURACION DE giroREACCIONES ////
      /////
      ////////////////////////////////////

      //Si despues de detectar izquierda, gira derecha, y detecta derecha giroreac=9, VOLVERA A GIRAR A derecha///
      Serial.println ("Activamos motores");
      motorON ();
      
      if (giroreac == 9) {
        //gira derecha
        derecha360 ();
        delay (250);
        giroreac = 0;
      }

      //9:SENSOR derecha DETECTA OBTACULO FRONTAL
    } else if (reaccion == 9) {
      Serial.println ("Activamos motores");
      motorON ();
      Serial.println ("Hacia atrÃ¡s");
      atras ();
      //Durante 3 segundos
      delay (200);

      Serial.println ("Hacia IZQUIERDA");
      izquierda360 ();
      //Durante 3 segundos
      delay (250);
      //-------------------------------------------------
      //Colocamos las bobinas en estado de apagado y LANZAMOS PINGS


      //LANZANDO PING DE DENTRO DE los giros, PARA USARLOS EN LAS MANIOBRAS Y HAGAN DE 'MEMORIA'

      //SENSOR Y LED IZQUIERDA
      ultizq ();

      Serial.print(cm);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      cm1 = cm;
      Serial.print("cm");
      Serial.println();
    

      if (cm1 < 25) {
        //        digitalWrite (sigled, HIGH);
        giroreac = 1;
    
      }
      else
      {
        //        digitalWrite (sigled, LOW);
        giroreac = 6;
      
      }
      //---------sensor derecha---------------

      //SENSOR Y LED DERECHA
      ultdcha ();

      Serial.print(espacio);
      //DEFINIR VARIABLE PARA QUE TOME EL VALOR DE CM Y ENCIENDA O APAGUE EL 'LED'
      int cm2 = espacio;
      Serial.print("cm");
      Serial.println();
 

      if (cm2 < 25) {
        //        digitalWrite (sigled2, HIGH);
        giroreac = giroreac + 3;
 
      }
      else
      {
        //        digitalWrite (sigled2, LOW);
        giroreac = giroreac + 4;

      }
      Serial.print(rereac);
      Serial.print("rereac");
   

      ////////////////////////////////////
      //CONFIGURACION DE giroREACCIONES ////
      /////
      ////////////////////////////////////

      //Si despues de detectar derecha, gira izqu, y detecta izq giroreac=5, VOLVERA A GIRAR A IZQUIERDA///
      Serial.println ("Activamos motores");
      motorON ();

     
      if (giroreac == 5) {

        Serial.println ("Hacia IZQUIERDA");
       izquierda360 ();
        //Durante 3 segundos
        delay (250);
        giroreac = 0;
      }
      //------------------------------------------------
      //10:AMBOS SENSORES NO DETECTAN ALERTAS
    } else if (reaccion == 10) {
      
      Serial.println ("Activamos motores");
      motorON ();

      //VELOCIDAD 1-MINIMA
      if ((cm1) && (cm2) <= 50) {
        
        alante ();
        //REGULAR VELOCIDAD EN DELAY
        delay (50);
       


        //VELOCIDAD 2-MEDIA
      } else if ((cm1) && (cm2) <= 250) {
        Serial.println ("Hacia delante");
        alante ();
        //AÃ‘ADIR DELAY PARA MAS VELOCIDAD
        delay (50);


        //VELOCIDAD 3-MAXIMA
      } else if ((cm1) && (cm2) >= 251) {
        Serial.println ("Hacia delante");
        alante ();
        //AÃ‘ADIR DELAY PARA MAS VELOCIDAD
        delay (50);

       
      }//FIN DEL ELSE IF VELOCIDAD 3
    }//FIN DEL ELSE IF QUE CONTIENE LA "REACCION 10"
   
   if (control==2 && lastcontrol==0){
     estado = 1;
     lastcontrol=control;
     Serial.print("estado");
     Serial.println(estado);
     motorOFF ();
     delay(1000);
     }//Fin del if control 2
         
  
    
  }//fin del ELSE (que pone el modo automatico)

}//fin del void loop
