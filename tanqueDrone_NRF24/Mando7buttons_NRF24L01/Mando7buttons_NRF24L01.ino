#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

//CONFIGURAMOS CANAL DE RADIO Y MENSAJE A ENVIAR
const int pinCE = 9;
const int pinCSN = 10;
RF24 radio(pinCE, pinCSN);
 
// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F4F0E1LL;
 
int data[1];

//CONFIGURAMOS PULSADORES 
int modalidad = 2;//PULSADOR PARA ACTIVAR O DESACTIVAR RC EN EL RECEPTOR, NO AFECTA A ESTE PARA NADA. ES PARA CREAR UN ENCENDIDO EN EL 'RECEPTOR'.
int luz = 3;// luz
int bocina =4;//pulsador bocina
int alante = 5;// alante
int atras = 6;// atras
int izquierda = 7;// izquierda
int derecha = 8;// derecha

 
void setup(){
    Serial.begin(9600);
    //INCIAMOS SERIAL Y RADIO
    radio.begin();
    radio.openWritingPipe(pipe);       // Abrir para escribir
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);

    //seteamos todos los pines indicando que todos son entradas menos el led
    pinMode (modalidad, INPUT);
    pinMode (luz, INPUT);
    pinMode (bocina, INPUT);
    pinMode (alante, INPUT);
    pinMode (atras, INPUT);
    pinMode (izquierda, INPUT);
    pinMode (derecha, INPUT);
}//Fin de "setup"

//Comienza bloque principal de ejecucion  
void loop(){

    if (digitalRead(modalidad)==HIGH){
        data[0] = 2;
        radio.write(data, sizeof(data));
        delay(1);
        //send("2")-modalidad;

    }else if (digitalRead(luz)==HIGH){
        data[0] = 3;
        radio.write(data, sizeof(data));
        //send("3")-luz;
        delay(1);

    }else if (digitalRead(bocina)==HIGH){
        data[0] = 4;
        radio.write(data, sizeof(data));
        //send("4")-bocina;
        delay(1);

    }else if ((digitalRead(alante)==HIGH)&&((digitalRead(izquierda)==LOW)&&(digitalRead(derecha)==LOW))){
        data[0] = 5;
        radio.write(data, sizeof(data));
        //send("5")-alante;
        delay(1);
  
    }else if ((digitalRead(atras)==HIGH)&&((digitalRead(izquierda)==LOW)&&(digitalRead(derecha)==LOW))){
        data[0] = 6;
        radio.write(data, sizeof(data));
        delay(1);
        //send("6")-atras;

    }else if ((digitalRead(izquierda)==HIGH)&&(((digitalRead(alante)==LOW)&&(digitalRead(luz)==LOW))&&(digitalRead(atras)==LOW))){
        data[0] = 7;
        radio.write(data, sizeof(data));
        //send("7")-izquierda;
        delay(1);

    }else if ((digitalRead(derecha)==HIGH)&&(((digitalRead(alante)==LOW)&&(digitalRead(luz)==LOW))&&(digitalRead(atras)==LOW))){
        data[0] = 8;
        radio.write(data,sizeof(data));
        //send("8")-derecha;
        delay(1);

    }else if ((digitalRead(alante)==HIGH)&&(digitalRead(izquierda)==HIGH)){
        data[0] = 9;
        radio.write(data, sizeof(data));
        //send("9")-alanteizquierda;
        delay(1);
  
    }else if ((digitalRead(alante)==HIGH)&&(digitalRead(derecha)==HIGH)){
        data[0] = 10;
        radio.write(data, sizeof(data));
        //send("10")-alantederecha;
        delay(1);

    }else if ((digitalRead(atras)==HIGH)&&(digitalRead(izquierda)==HIGH)){
        data[0] = 11;
        radio.write(data, sizeof(data));
        //send("11")-atrasizquierda;
        delay(1);

    }else if ((digitalRead(atras)==HIGH)&&(digitalRead(derecha)==HIGH)){
        data[0] = 12;
        radio.write(data,sizeof(data));
        //send("12")-atrasderecha;
        delay(1);
 
    }else{
        data[0] = 0;
        radio.write(data, sizeof(data));
        delay(1);
    
    }//Fin de ELSE-IF


    Serial.println(data[0]);
 
}//Fin del bloque principal "loop"
