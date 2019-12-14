//Importamos librerias
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


//CONFIGURAMOS PULSADORES (ASIGNANDO NOMBRE A PINES) 
int modalidad = 2; //Pulsador para Activar/Desactivar modo automatico
int luz = 3;       //Pulsador para activar luz
int bocina =4;     //Pulsador activar bocina
int alante = 5;    //Pulsador alante
int atras = 6;     //Pulsador atras
int izquierda = 7; //Pulsador izquierda
int derecha = 8;   //Pulsador derecha


//Inicio "setup", Inicializamos todo aquí 
void setup(){

    //INCIAMOS SERIAL Y RADIO
    Serial.begin(9600);
    
    radio.begin();
    radio.openWritingPipe(pipe);     // Abrir para escribir
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_250KBPS);

    //seteamos todos los pines indicando si son de Entrada o Salida
    pinMode (modalidad, INPUT); //Salida
    pinMode (luz, INPUT);       //Salida
    pinMode (bocina, INPUT);    //Salida
    pinMode (alante, INPUT);    //Salida
    pinMode (atras, INPUT);     //Salida
    pinMode (izquierda, INPUT); //Salida
    pinMode (derecha, INPUT);   //Salida

}//Fin de "setup"



//Comienza bloque principal de ejecucion  
void loop(){

    //Según que boton/es este pulsado se enviará un mensaje u otro
    if (digitalRead(modalidad)==HIGH){
        data[0] = 2;
        radio.write(data, sizeof(data));
        //send("2")-modalidad;

    }else if (digitalRead(luz)==HIGH){
        data[0] = 3;
        radio.write(data, sizeof(data));
        //send("3")-luz;
        

    }else if (digitalRead(bocina)==HIGH){
        data[0] = 4;
        radio.write(data, sizeof(data));
        //send("4")-bocina;


    }else if ((digitalRead(alante)==HIGH)&&((digitalRead(izquierda)==LOW)&&(digitalRead(derecha)==LOW))){
        data[0] = 5;
        radio.write(data, sizeof(data));
        //send("5")-alante;

  
    }else if ((digitalRead(atras)==HIGH)&&((digitalRead(izquierda)==LOW)&&(digitalRead(derecha)==LOW))){
        data[0] = 6;
        radio.write(data, sizeof(data));
        //send("6")-atras;

    }else if ((digitalRead(izquierda)==HIGH)&&(((digitalRead(alante)==LOW)&&(digitalRead(luz)==LOW))&&(digitalRead(atras)==LOW))){
        data[0] = 7;
        radio.write(data, sizeof(data));
        //send("7")-izquierda;


    }else if ((digitalRead(derecha)==HIGH)&&(((digitalRead(alante)==LOW)&&(digitalRead(luz)==LOW))&&(digitalRead(atras)==LOW))){
        data[0] = 8;
        radio.write(data,sizeof(data));
        //send("8")-derecha;


    }else if ((digitalRead(alante)==HIGH)&&(digitalRead(izquierda)==HIGH)){
        data[0] = 9;
        radio.write(data, sizeof(data));
        //send("9")-alanteizquierda;

  
    }else if ((digitalRead(alante)==HIGH)&&(digitalRead(derecha)==HIGH)){
        data[0] = 10;
        radio.write(data, sizeof(data));
        //send("10")-alantederecha;


    }else if ((digitalRead(atras)==HIGH)&&(digitalRead(izquierda)==HIGH)){
        data[0] = 11;
        radio.write(data, sizeof(data));
        //send("11")-atrasizquierda;


    }else if ((digitalRead(atras)==HIGH)&&(digitalRead(derecha)==HIGH)){
        data[0] = 12;
        radio.write(data,sizeof(data));
        //send("12")-atrasderecha;

 
    }else{
        data[0] = 0;
        radio.write(data, sizeof(data));

    
    }//Fin de ELSE-IF


    Serial.println(data[0]);
 
}//Fin del bloque principal "loop"
