//RECEPTOR NRF24L01
///ARDUINO MEGA

///Agregando librerias
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


///configurando RF 2,4Ghz
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);

/// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F4F0E1LL;

int data[1]; //Declarando array del que se compondrá el mensaje
int control=0; //Variable en la que se almacenará el dato que contendrá el mensaje recibido "data[0]"
int lastcontrol=0; //variable que tomará el ultimo valor de "control" servirá para hacer comprobaciones



///MIENTRAS "ESTADO ES 1" LA MAQUINA ESTARA EN MODO AUTO Y EL LED ESTARA PRENDIDO, SI SE VUELVE 0 (MANUAL), EL LED SE APAGARA
int estado = 0;//Variable que determinara el modo MANUAL o AUTOMATICO
int estadoAnt=estado;

//int ledestado = A0;//No estamos usando led todavia


/***********************************************************************
*** Esta variable "PANIC OFF" será un apagado de emergencia,         ***
*** se utiliza de forma independiente en el modo AUTO o MANUAL       ***
*** para que automaticamente no camine si pierde la señal del mando  ***
*** y PODEMOS ELIMINARLA de alguna de las funciones si queremos.     ***
*** ya que mientras exista en el bloque correspondiente              ***
*** EL ROBOT NO FUNCIONARA SIN MANDO.                                ***
*** Se encontraran justo al inicio de cada modalidad con el TITULO   ***
***  "APAGADO DE EMERGENCIA" como nombre en el comentario superior   ***/
int panicOff_NoCobertura=0;//Esta variable sera un APAGADO DE EMERGENCIA por si nos quedamos sin cobertura del mando, sera un contador

///COLOCAMOS TODAS LAS ENTRADAS Y SALIDAS DE SENSORES Y SUS NUMEROS DONDE LOS CONECTAMOS
///ULTRASONIDO-LED DERECHA
//int sigled2 = A2; ///led prueba rojo- DERECHA
int trigPin2 = 30;    ///Trig - ROJO
int echoPin2 = 31;    ///Echo - BLANCO

///ULTRASONIDO-LED IZQUIERDA
//int sigled = 13; ///led prueba ROJO-IZQUIERDA
int trigPin1 = 28;    ///Trig - ROJO
int echoPin1 = 29;    ///Echo - BLANCO

///VARIABLES PARA DETERMINAR LA DISTANCIA, AGREGAR 3 MAS PARA CADA SENSOR
long duration, cm, inches, duracion, espacio, nose;

///MOTORES
///Motor A
int enableA = 22;///CONTROL-A (Enciende o apaga el motor)
int motorA1 = 23;///BOBINA-A1
int motorA2 = 24;///BOBINA-A2

///Motor B
int enableB = 25;///CONTROL-B (Enciende o apaga el motor)
int motorB1 = 26;///BOBINA-B1
int motorB2 = 27;///BOBINA-B2

///variables para REREACCIONES!!!
int rereac = 0;
long reduration, recm, reinches, reduracion, reespacio, renose;
int giroreac = 0;

///Inicializando radio, pines, serial, etc
void setup() {
    ///Serial Port begin y RF 2,4Ghz
    radio.begin();
    Serial.begin(9600);
    radio.openReadingPipe(1, pipe);
    radio.startListening();
    radio.setPALevel(RF24_PA_HIGH); 
    radio.setDataRate(RF24_250KBPS);

    ///Seteamos los pines como entradas O salidas
    
    ///ECHO-IZQUIERDA
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    //pinMode(sigled, OUTPUT);
    
    ///ECHO-DERECHA
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    //pinMode(sigled2, OUTPUT);
    
    ///MOTOR-DERECHA
    pinMode (enableA, OUTPUT); 
    pinMode (motorA1, OUTPUT);
    pinMode (motorA2, OUTPUT);
  
    ///MOTOR-IZQUIERDA
    pinMode (enableB, OUTPUT); 
    pinMode (motorB1, OUTPUT);
    pinMode (motorB2, OUTPUT);
}///Fin del SETUP


////////////////////////////////////
//// configuramos acciones de motor para simplificar el codigo
///////////////////////////////////


///ACTIVAMOS MOTORES A(motor derecho) y B(motor izquierdo)
void MotorOn () {
    digitalWrite (enableA, HIGH);
    digitalWrite (enableB, HIGH);
}///Fin MotorOn


///solo la rueda derecha gira hacia Atras (el robot girará a la izquierda y Atras)
void IzqAtras () {
    digitalWrite (motorB1, LOW); 
    digitalWrite (motorB2, LOW);
    digitalWrite (motorA1, LOW); 
    digitalWrite (motorA2, HIGH);
}///Fin IzqAtras

///solo la rueda derecha gira hacia Alante (el robot girará a la izquierda y Alante)
void IzqAlante () {
    digitalWrite (motorB1, LOW); 
    digitalWrite (motorB2, LOW);
    digitalWrite (motorA1, HIGH);
    digitalWrite (motorA2, LOW);
}///Fin IzqAlante

//solo la rueda izquierda gira hacia Atras (el robot girará a la derecha y Atras)
void DchaAtras () {
    digitalWrite (motorA1, LOW); 
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, LOW); 
    digitalWrite (motorB2, HIGH);
}//Fin DchaAtras

///solo la rueda izquierda gira hacia Alante (el robot girará a la derecha y Alante)
void DchaAlante () {
    digitalWrite (motorA1, LOW); 
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, HIGH); 
    digitalWrite (motorB2, LOW);
}///Fin DchaAlante

///Atras
void Atras () {
    digitalWrite (motorA1, LOW);
    digitalWrite (motorA2, HIGH);
    digitalWrite (motorB1, LOW);
    digitalWrite (motorB2, HIGH);
}///Fin Atras

///Alante
void Alante () {
    digitalWrite (motorA1, HIGH);
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, HIGH);
    digitalWrite (motorB2, LOW);
}///Fin Alante

///GIRAR Izquierda360
void Izquierda360 () {
    digitalWrite (motorA1, HIGH);
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, LOW);
    digitalWrite (motorB2, HIGH);
}///Fin Izquierda360

/// GIRAR Derecha360
void Derecha360 () {
    digitalWrite (motorA1, LOW); 
    digitalWrite (motorA2, HIGH);
    digitalWrite (motorB1, HIGH); 
    digitalWrite (motorB2, LOW);
}///Fin Derecha360

///PARAR MOTORES
void MotorOff () {
    digitalWrite (enableA, LOW);
    digitalWrite (enableB, LOW);
    digitalWrite (motorA1, LOW);
    digitalWrite (motorA2, LOW);
    digitalWrite (motorB1, LOW);
    digitalWrite (motorB2, LOW);
}///Fin MotorOff


///metodos para ULTRASONIDOS

void UltIzq (){
    ///SENSOR hc-sr04 IZQUIERDA ultrasonido
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    pinMode(echoPin1, INPUT);
    duration = pulseIn(echoPin1, HIGH);
    /// convert the time into a distance
    cm = (duration / 2) / 27.1;
    inches = (duration / 2) / 74;
}///Fin UltIzq

void UltDcha (){
   ///SENSOR hc-sr04 DERECHA
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    pinMode(echoPin2, INPUT);
    duracion = pulseIn(echoPin2, HIGH);
    /// convert the time into a distance
    espacio = (duracion / 2) / 27.1;
    nose = (duracion / 2) / 74;
}///Fin UltDcha



///COMIENZA el BLOQUE PRINCIPAL DE EJECUCION
void loop(){   
    
    ///Si se reciben los datos del emisor, almacenaremos el dato recibido en la var "control" y trabajaremos con esta
    if (radio.available()){
        radio.read(data, sizeof(data));
        Serial.println(data[0]);
        control = data[0];
        panicOff_NoCobertura=0;
    }else{
        ///Aqui sumaremos un contador, ira sumando si se pierde la senial del mando
        panicOff_NoCobertura++;
    }///FIN IF (radio.available)

    
    /***********************************************************************
    *** COMPROBACION asegurarnos que no cambie "estados"           ********* 
    *** por "REPETICION DEL MENSAJE.                               *********
    *** HASTA QUE NO VOLVAMOS A RECIBIR UN 0, DESPUES DE UNA SERIE DE 2, ***
    *** Forzaremos CAMBIAR "control" a 0                           *********
    ***********************************************************************/
    ///Si control es 0 y lastcontrol es 2(cambiado en metodos inferiores cuando realiza el cambio de estado), "lastcontrol" volverá a valer 0 y dejará de modificar el valor de "control" 
    if (control==0 && lastcontrol==2){
        lastcontrol=control;
    }///Fin del if
    
    ///Mientras "lastcontrol" sea 2, pondremos"control"a 0 hasta que el dato a enviar cambie de ser 2
    if (lastcontrol==2){
        control=0;
    }///Fin del if
    
    ///Si (control==2)
    if (control == 2 && lastcontrol==0){

        ///Segun que valor tenga estado, se cambiara a uno u otro
        if (estadoAnt==1){
            estado = 0;
        }else {
            estado=1;
        }///Fin del If-Else
        
        lastcontrol=control;///cambiando variable lastcontrol al valor de control
        MotorOff();                         
    }///Fin del if


    /*** FIN DE EVITAR REBOTE DEL BOTON DEL EMISOR************************************/

///Según la variable "estado", correrá el modo automatico (estado==1), o el modo manual(estado==0)  
    
    ///===================ENTRAMOS EN EL MODO MANUAL================================================================
    if (estado == 0){
        estadoAnt=0;
        
        ///*************APAGADO DE EMERGENCIA*******************************************************
        ///Si el "panicOff_NoCobertura" llega a 10000, pondremos el "control a 0", por si se perdio la conexion con el emisor no siga caminando solo
        if (panicOff_NoCobertura==10000){
            control=0;
            panicOff_NoCobertura=0;
        }///Fin del if
        ///*************FIN APAGADO DE EMERGENCIA*******************************************************
        
        
        ///SE APAGA EL LED VERDE, SE INICIA MODO MANUAL
        //digitalWrite (ledestado, LOW);

      
        /////////////////////////////////////////////////////
        ////ACTUANDO SEGUN DATOs RECIBIDOs DEL MANDO RC//////
        /////////////////////////////////////////////////////

     

        if (control == 5) {
            ///Alante 
            MotorOn ();
            Alante ();
           

        } else if (control == 3) {

           //HAY QUE CREAR LA FUNCION PARA ACTIVAR UNA LUZ


        } else if (control == 6) {
            ///Atras
            MotorOn ();
            Atras ();
          

        } else if (control == 8) {
            MotorOn ();
            ///girar en 360 hacia derecha
            Derecha360 ();
  

        } else if (control == 7) {
            MotorOn ();
            ///girar en 360 hacia izquierda
            Izquierda360 ();
           

        } else if (control == 11) {
            MotorOn ();
            ///solo la rueda  derecha gira hacia Atras
            IzqAtras ();
            

        } else if (control == 12) {
            MotorOn ();
            ///solo la rueda izquierda gira hacia Atras
            DchaAtras ();
            

        } else if (control == 10) {
            MotorOn();
            ///solo la rueda izquierda gira hacia delante a velocidad1
            DchaAlante();
            

        } else if (control == 4) {
        
            //HAY QUE CREAR LA FUMCION, PARA LA BOCINAAAA!!!
            

        } else if (control == 9) {
            MotorOn();
            ///solo la rueda derecha gira hacia delante a velocidad1
            IzqAlante();
            
        }else {
            ///paramos motores
            MotorOff ();    
     
        }///Fin ELSE-IF que compara que se está recibiendo
     
    ///=======FINAL de MODO MANUAL "(si estado es 0)"=================================================================================

//####################################################################################################################

    ///=======ENTRANDO EN MODO AUTOMATICO===================================================================================

    }else if (estado==1) { ///SI "ESTADO" ES 1, COMENZARa EL MODO AUTOMATICO.
        
        estadoAnt=1;
    ///SE ENCENDERA EL LED VERDE EN MODO AUTOMATICO
  //digitalWrite (ledestado, HIGH);

    ///*************APAGADO DE EMERGENCIA*******************************************************
    ///Si ""panicOff_NoCobertura" llega a 2, pondremos el "control a 0", por si se perdio la conexion con el emisor no siga caminando solo
    if (panicOff_NoCobertura==2){
        control=0;
        panicOff_NoCobertura=0;
        estado=0;///Pondremos "estado" a 0, para que el robot entre en modo "MANUAL" y se quede parado
    }///Fin del if
    ///*************FIN de APAGADO DE EMERGENCIA************************************************    
        

    ///VARIABLE PARA ACCION DE MOTOR Y SENSORES, IRA DE 0 A 4, TENDRA 4 ACCIONES Y UN ESTADO INICIAL SIEMPRE VUELVE A 0 AL FINAL DEL LOOP
    int reaccion = 0;

    ///SENSOR Y LED IZQUIERDA
    UltIzq ();
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
    
    UltDcha ();
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
      
      MotorOn ();
      Serial.println ("Hacia Atras");
      Atras ();
      delay (250);
      
      //EL ROBOT GIRARÃ� HACIA DONDE HALLA MAS SITIO LIBRE COMPARANDO LA DISTANCIA MEDIDA ENTRE LOS DOS SENSORES
      if (cm1 <= cm2) {

        Derecha360 ();
        delay (250);

      } else {
        Serial.println ("Hacia IZQUIERDA");
        Izquierda360 ();
        delay (250);
      }
      /////////////////////////////////////////////////////////////////////
      //1111111111....11111111111111111--------CREANDO  REREACCIONEEESSS!!//
      //////////////////////////////////////////////////////////////////////
      //Colocamos las bobinas en estado de apagado y LANZAMOS PINGS


      //LANZANDO PING DE DENTRO DE LA ALERTA-4, PARA USARLOS EN LAS MANIOBRAS Y HAGAN DE 'MEMORIA'

      //SENSOR Y LED IZQUIERDA
      UltIzq();
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
      UltDcha ();

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
      
      MotorOn ();
      
      if (rereac == 4) {
        //EL ROBOT GIRARÃ� HACIA DONDE HALLA MAS SITIO LIBRE COMPARANDO LA DISTANCIA MEDIDA ENTRE LOS DOS SENSORES
        if (cm1 <= cm2) {

          Derecha360 ();
          //TIEMPO DE GIRO
          delay (250);

        } else {

          Serial.println ("Hacia IZQUIERDA");
          Izquierda360 ();
          //TIEMPO DE GIRO
          delay (250);
        }

        //SENSOR IZQUIERDA-- SI DETECTA A LA IZQ, GIRARA A LA DERECHA
      } else if (rereac == 5) {
        Derecha360 ();
        //Durante 3 segundos
        delay (250);

        //SENSOR DERECHA-- SI DETECTA A LA DERECHA GIRARA A IZQ.
      } else if (rereac == 9) {
        Serial.println ("Hacia IZQUIERDA");
        Izquierda360 ();
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
      UltIzq ();

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
      UltDcha ();
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
      MotorOn ();

      //37=  va hacia Atras
      if (rerereac == 37) {

        if (cm1 <= cm2) {

          Derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
          Izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }

        //40=  gira derecha
      } else if (rerereac == 40) {
        Derecha360 ();
        //Durante 3 segundos
        delay (350);
        
        //61= gira izquierda
      } else if (rerereac == 61) {
        Izquierda360 ();
        //Durante 3 segundos
        delay (350);
       
        //38=  compara cm1 y cm2 y gira
      } else if (rerereac == 38) {

        if (cm1 <= cm2) {

          Derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
          Izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }
       
        //41=  si
      } else if (rerereac == 41) {

        if (cm1 >= cm2) {

         Derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
         Izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }
        //62=  gira izquierda
      } else if (rerereac == 62) {
        Serial.println ("Hacia IZQUIERDA");
        Izquierda360 ();
        delay (350);
      
        //42= compara cm1 y cm2 y gira
      } else if (rerereac == 42) {

        if (cm1 <= cm2) {

          Derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
         Izquierda360 ();
          //Durante 3 segundos
          delay (350);
        }
      
        //45= gira derecha
      } else if (rerereac == 45) {
        Derecha360 ();
        //Durante 3 segundos
        delay (350);
       
        //66 = compara y decide solo donde girar
      } else if (rerereac == 66) {

        if (cm1 >= cm2) {

          Derecha360 ();
          //Durante 3 segundos
          delay (350);

        } else {
          Serial.println ("Hacia IZQUIERDA");
          Izquierda360 ();
          delay (350);
        }
      }

      /////////////////////////////////////////////////////////////////////////////////
      //FIN DE REREREACCIONES ALERTA-4///////////////
      //===================================================================================================

      //5:SENSOR izquierda DETECTA OBTACULO
    } else if (reaccion == 5) {
      //Atras
      Serial.println ("Activamos motores");
      MotorOn ();
      
      Serial.println ("Hacia atrÃ¡s");
      Atras ();
      //Durante 3 segundos
      delay (200);

      // GIRAR DERECHA
      Derecha360 ();
      //Durante 3 segundos
      delay (250);
  

      //LANZANDO PING DE DENTRO DE los giros, PARA USARLOS EN LAS MANIOBRAS Y HAGAN DE 'MEMORIA'

      //SENSOR Y LED IZQUIERDA
      UltIzq ();

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
      UltDcha ();

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
      MotorOn ();
      
      if (giroreac == 9) {
        //gira derecha
        Derecha360 ();
        delay (250);
        giroreac = 0;
      }

      //9:SENSOR derecha DETECTA OBTACULO FRONTAL
    } else if (reaccion == 9) {
      Serial.println ("Activamos motores");
      MotorOn ();
      Serial.println ("Hacia atrÃ¡s");
      Atras ();
      //Durante 3 segundos
      delay (200);

      Serial.println ("Hacia IZQUIERDA");
      Izquierda360 ();
      //Durante 3 segundos
      delay (250);
      //-------------------------------------------------
      //Colocamos las bobinas en estado de apagado y LANZAMOS PINGS


      //LANZANDO PING DE DENTRO DE los giros, PARA USARLOS EN LAS MANIOBRAS Y HAGAN DE 'MEMORIA'

      //SENSOR Y LED IZQUIERDA
      UltIzq ();

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
      UltDcha ();

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
      MotorOn ();

     
      if (giroreac == 5) {

        Serial.println ("Hacia IZQUIERDA");
       Izquierda360 ();
        //Durante 3 segundos
        delay (250);
        giroreac = 0;
      }
      //------------------------------------------------
      //10:AMBOS SENSORES NO DETECTAN ALERTAS
    } else if (reaccion == 10) {
      
      Serial.println ("Activamos motores");
      MotorOn ();

      //VELOCIDAD 1-MINIMA
      if ((cm1) && (cm2) <= 50) {
        
        Alante ();
        //REGULAR VELOCIDAD EN DELAY
        delay (50);
       


        //VELOCIDAD 2-MEDIA
      } else if ((cm1) && (cm2) <= 250) {
        Serial.println ("Hacia delante");
        Alante ();
        //AÃ‘ADIR DELAY PARA MAS VELOCIDAD
        delay (50);


        //VELOCIDAD 3-MAXIMA
      } else if ((cm1) && (cm2) >= 251) {
        Serial.println ("Hacia delante");
        Alante ();
        //AÃ‘ADIR DELAY PARA MAS VELOCIDAD
        delay (50);

       
      }//FIN DEL ELSE IF VELOCIDAD 3
    }//FIN DEL ELSE IF QUE CONTIENE LA "REACCION 10"
   
    }//fin del ELSE (que pone el modo automatico)

}//fin del void loop
