/**
  * Control de una mano Robot con Servos y Joysticks usando Arduino.
  * 
  * Autor: David Álvarez Castro
  * Mail: davidac0291@gmail.com
  * Fecha: 12/01/2019
  * Version: 1.0.0
  * 
  **/

#include <Servo.h> // Librería para el control de los servos

/****************************  CONSTANTES DE LA APLICACIÓN ****************************/
// PIN analógicos para leer datos
const int analogInGruaElevar = A1;
const int analogInGruaExtender = A4;
const int analogInPinza = A5;
const int analogInRotar = A0;
// PIN digitales para escribir datos
const int digitalOutElevar = 3;
const int digitalOutExtender = 5;
const int digitalOutRotar = 2;
const int digitalOutPinzas = 4;

// Controladores de los servos
Servo servoElevar, servoExtender,
      servoPinzas, servoRotar;

int margenLecturaAnalogico = 5; // Margen de tolerancia para los valores analógicos obtenidos

// Configuración para los límites de rotación de los servos
int gradosServoGeneral = 0; // Inicial Servo
int minGradosServoGeneral = 0; // Min Servo
int maxGradosServoGeneral = 180; // Max Servo
// Inicial Elevar
int gradosGruaElevar = 50;
int minGradosGruaElevar = minGradosServoGeneral;
int maxGradosGruaElevar = 150;
// Inicial Extender
int gradosGruaExtender = 100;
int minGradosGruaExtender = 10;
int maxGradosGruaExtender = 130;
// Inicial Pinzas
int gradosPinzas = 20;
int minGradosPinzas = 10;
int maxGradosPinzas = maxGradosServoGeneral;
// Inicial Rotación
int gradosRotar = 90;
int minGradosRotar = minGradosServoGeneral;
int maxGradosRotar = maxGradosServoGeneral;
/********************************* FIN CONSTANTES *********************************/


/** 
  * Función encargada de gestionar los servos
  *
  * @param nombre: nombre del servo para mostrar por pantalla.
  * @param servo: instacia del servo a modificar.
  * @param inAnalogico: entrada analógica en donde leer los datos de movimiento.
  * @param minGradosServo: mínimo grado de movimiento del servo.
  * @param initGradosServo: estado inicial del servo (valor del grado inicial).
  * @param maxGradosServo: máximo grado de movimiento del servo.
  * @param margenAnalogico: margen dado para la lectura de datos analógicos (el dato no es exacto y siempre oscila).
  * 
  * @returns void
  *
  * */
void moverServo (String nombre, Servo servo, int inAnalogico, int outDigital, int minGradosServo, int* initGradosServo, int maxGradosServo, int margenAnalogico) {
  int valorSensor = 0;
  int* gradosServo; // Puntero para la modificación de la variable global que almacena la posición del servo
  String msg = nombre;

  gradosServo = initGradosServo;
  msg += String(" ==> ");

  // Leemos el valor analógico obtenido del joystick y lo mapeamos a valores propios
  valorSensor = analogRead(inAnalogico);
  valorSensor = map(valorSensor, 0, 1023, 125, -125);
  
  msg += String(valorSensor);
  msg += String(" => ");

  // Gestionamos el valor obtenido => obtenemos el grado a partir de los valores del joystick
  if (valorSensor > margenAnalogico) {
    *gradosServo = *gradosServo - 1;
   } else if (valorSensor < -margenAnalogico) {
    *gradosServo = *gradosServo + 1;
   }

   // Gestionamos los límites de giro del servo 
   if (*gradosServo > maxGradosServo) {
    *gradosServo = maxGradosServo;
   } else if (*gradosServo < minGradosServo) {
    *gradosServo = minGradosServo;
   }

  // Movemos el servo enviando un grado determinado
  servo.write(*gradosServo);

  msg += String(*gradosServo);
  Serial.println(msg);
}

/** 
  * Setup: inicialización
  * 
  * */
void setup() {
  Serial.begin(9600);
  // Asociamos cada uno de los servos con la entrada digital correspondiente
  servoElevar.attach(digitalOutElevar);
  servoPinzas.attach(digitalOutPinzas);
  servoRotar.attach(digitalOutRotar);
  servoExtender.attach(digitalOutExtender);

  // Inicializamos los servos (estado inicial)
  servoElevar.write(gradosGruaElevar);
  servoPinzas.write(gradosPinzas);
  servoRotar.write(gradosRotar);
  servoExtender.write(gradosGruaExtender);
}

/** 
 * Bucle principal
 * 
 * */
void loop() {
  moverServo (String("Extender"), servoExtender, analogInGruaExtender, digitalOutExtender, minGradosGruaExtender,
    &gradosGruaExtender, maxGradosGruaExtender, margenLecturaAnalogico);
  moverServo (String("Elevar"), servoElevar, analogInGruaElevar, digitalOutElevar, minGradosGruaElevar,
    &gradosGruaElevar, maxGradosGruaElevar, margenLecturaAnalogico);
  moverServo (String("Rotar"), servoRotar, analogInRotar, digitalOutRotar, minGradosRotar,
    &gradosRotar, maxGradosRotar, margenLecturaAnalogico);
  moverServo (String("Pinzas"), servoPinzas, analogInPinza, digitalOutPinzas, minGradosPinzas,
    &gradosPinzas, maxGradosPinzas, margenLecturaAnalogico);
  delay(15);
}
