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

//CONFIGURAMOS PULSADOR Y JOYSTICK
int modalidad = 2;//PULSADOR PARA ACTIVAR O DESACTIVAR RC EN EL RECEPTOR, NO AFECTA A ESTE PARA NADA. ES PARA CREAR UN ENCENDIDO EN EL 'RECEPTOR'.
int atras = 6;//joystick atras
int alante1 = 5;//joystick alante1
int alante2 = 3;//joystick alante2
int derecha = 8;//joystick derecha
int izquierda = 7;//joystick izquierda
   //FALTA UN PULSADOR QUE NO SE AGREGO TODAVIA.
 
void setup(){
  Serial.begin(9600);
  //INCIAMOS SERIAL Y RADIO
  radio.begin();
  radio.openWritingPipe(pipe);       // Abrir para escribir
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);

//seteamos todos los pines indicando que todos son entradas menos el led
  pinMode (modalidad, INPUT);
  pinMode (atras, INPUT);
  pinMode (alante1, INPUT);
  pinMode (alante2, INPUT);
  pinMode (derecha, INPUT);
  pinMode (izquierda, INPUT);
}
  
void loop(){
 
 //fALTA ARREGLAR TROZOS DEL CODIGO

 if (digitalRead(modalidad)==HIGH){
  
  data[0] = 22;
  radio.write(data, sizeof(data));
  delay(1);
  
  //send("encender");
  
  
}else if ((digitalRead(atras)==HIGH)&&(digitalRead(derecha)==HIGH)){
  
  data[0] = 4;
  radio.write(data,sizeof(data));
  //send("atrasderecha");
  delay(1);
 
 }else if ((digitalRead(atras)==HIGH)&&(digitalRead(izquierda)==HIGH)){

  data[0] = 5;
  radio.write(data, sizeof(data));
 // send("atrasizquierda");
   delay(1);
 
 }else if ((digitalRead(atras)==HIGH)&&((digitalRead(izquierda)==LOW)&&(digitalRead(derecha)==LOW))){

  data[0] = 6;
  radio.write(data, sizeof(data));
  delay(1);
 // send("atras");

  
 }else if ((digitalRead(alante1)==HIGH)&&((digitalRead(izquierda)==LOW)&&(digitalRead(derecha)==LOW))){

  data[0] = 7;
  radio.write(data, sizeof(data));
  //send("alante1");
  delay(1);
  
 }else if ((digitalRead(alante2)==HIGH)&&((digitalRead(izquierda)==LOW)&&(digitalRead(derecha)==LOW))){

  data[0] = 8;
  radio.write(data, sizeof(data));
  //send("alante2");
  delay(1);
  
 }else if ((digitalRead(alante1)==HIGH)&&(digitalRead(izquierda)==HIGH)){

  data[0] = 9;
  radio.write(data, sizeof(data));
  //send("alanteizquierda1");
 delay(1);
  
 }else if ((digitalRead(alante2)==HIGH)&&(digitalRead(izquierda)==HIGH)){

  data[0] = 10;
  radio.write(data, sizeof(data));
  //send("alanteizquierda2");
  delay(1);
  
 }else if ((digitalRead(alante1)==HIGH)&&(digitalRead(derecha)==HIGH)){

  data[0] = 11;
  radio.write(data, sizeof(data));
  //send("alantederecha1");
 delay(1);
  
 }else if ((digitalRead(alante2)==HIGH)&&(digitalRead(derecha)==HIGH)){

  data[0] = 12;
  radio.write(data, sizeof(data));
  //send("alantederecha2");
  delay(1);


  //GIROS EN EL SITIO A IZQUIERDA Y DERECHA
 }else if ((digitalRead(derecha)==HIGH)&&(((digitalRead(alante1)==LOW)&&(digitalRead(alante2)==LOW))&&(digitalRead(atras)==LOW))){

  data[0] = 13;
  radio.write(data,sizeof(data));
  //send("derecha");
 delay(1);

 }else if ((digitalRead(izquierda)==HIGH)&&(((digitalRead(alante1)==LOW)&&(digitalRead(alante2)==LOW))&&(digitalRead(atras)==LOW))){
 
  data[0] = 14;
  radio.write(data, sizeof(data));
  //send("izquierda");
 delay(1);


  }else {// ((digitalRead(izquierda)==LOW)&&(((digitalRead(alante1)==LOW)&&(digitalRead(alante2)==LOW))&&(digitalRead(atras)==LOW))){

  data[0] = 14;
  radio.write(data, sizeof(data));
 delay(1);
  }
Serial.println(data[0]);
 
}
