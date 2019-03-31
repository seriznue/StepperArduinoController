#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

#define IN5 8
#define IN6 9
#define IN7 10
#define IN8 11

int Paso [ 8 ][ 4 ] =
  { { 1, 0, 0, 0},
  { 1, 1, 0, 0},
  { 0, 1, 0, 0},
  { 0, 1, 1, 0},
  { 0, 0, 1, 0},
  { 0, 0, 1, 1},
  { 0, 0, 0, 1},
  { 1, 0, 0, 1}
  };

struct Motor {
  int steps; // Define el paso actual de la secuencia
  float angle;
  const int analogPin;
  const int pin1;
  const int pin2;
  const int pin3;
  const int pin4;
  int value; //variable que almacena la lectura analógica raw
  int position; //posicion del potenciometro en tanto por ciento   
  Motor(int aPin, int dPin1, int dPin2, int dPin3, int dPin4):
    steps(0),
    angle(0.0),
    analogPin(aPin),
    pin1(dPin1),
    pin2(dPin2),
    pin3(dPin3),
    pin4(dPin4),
    value(0),
    position(0){}
};

//int position; //posicion del potenciometro en tanto por ciento
Motor motorH(A0, IN1, IN2, IN3, IN4);
Motor motorV(A1, IN5, IN6, IN7, IN8);

float steps_per_round = (64*64);
float angle_paso = 360.0/steps_per_round;
float delayValue = 0;
String cadenaCharEntrada = "";  
char vectorChar[20];
bool finCadena = false;

void setup()
{
  Serial.begin(9600);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  
  cadenaCharEntrada.reserve(20);
}

void loop()
{
  delayValue = 0; 
  
  if(finCadena) {                               //Se termino de recibir una cadena completa
    
    finCadena = false;                                  //Permitimos volver a recibir otra cadena
    cadenaCharEntrada.toCharArray(vectorChar,20);   //Convertimos el string a un vectorChar
    int numeroEntero = atoi(vectorChar);            //Convertimos un vectorChar a un entero
   
    if(numeroEntero == 1){
      motorH.position = 99;
      doStep(motorH, true); // Avanza un paso
    }
    else if(numeroEntero == 2){
      motorH.position = 1;
      doStep(motorH, false); // Avanza un paso
    }
    if(numeroEntero == 4){
      motorV.position = 99;
      doStep(motorV, true); // Avanza un paso
    }
    else if(numeroEntero == 5){
      motorV.position = 1;
      doStep(motorV, false); // Avanza un paso
    }
    else if(numeroEntero == 3){
      printSteppersInfo();  
      delay (5) ; 
    }    
    cadenaCharEntrada = "";
  } 
  else {  
    moveMotorFromAnalogSignal(motorH);
    moveMotorFromAnalogSignal(motorV);
    
    delayValue = 100/(float)(max( abs((motorH.position - 50)) , abs((motorV.position - 50))) );    
    delay (delayValue) ; //Un paso cada 250 ms para ver los leds
  }
}

void moveMotorFromAnalogSignal(Motor &motor) {
  motor.value = analogRead(motor.analogPin); // realizar la lectura analógica raw
  motor.position = map(motor.value, 0, 1023, 1, 99); // convertir a porcentaje
  
  if(motor.position > 55 || motor.position < 45) {      
    if(motor.position == 0) {
      motor.position = 1;      
    }
    else if(motor.position == 100) {
      motor.position = 99;
    }
     
    bool forward = motor.position > 50;
    doStep(motor, forward); // Avanza un paso
  } 
} //Avanza un paso 


void doStep(Motor &motor, bool forward) //Avanza un paso                                                                                                                                 
{
  digitalWrite( motor.pin1, Paso[motor.steps % 8][ 0] );  
  digitalWrite( motor.pin2, Paso[motor.steps % 8][ 1] );  
  digitalWrite( motor.pin3, Paso[motor.steps % 8][ 2] );  
  digitalWrite( motor.pin4, Paso[motor.steps % 8][ 3] );
  
  if (forward){  
    motor.steps++;
    motor.angle += angle_paso; 
  }
  else {
    motor.steps--;
    motor.angle -= angle_paso;
  }

  if(motor.angle < 0.0) {
    motor.angle +=360.0;
  } 
  if(motor.angle > 360.0) {
    motor.angle -= 360.0;
  }
  if(motor.steps < 0.0) {
    motor.steps += steps_per_round;
  } 
  if(motor.steps > steps_per_round) {
    motor.steps -= steps_per_round;
  }
}

void printSteppersInfo() {
  Serial.print("{");
  //List
  Serial.print("\"steppers\":[");
  //Stepper 1
  printStepperInfo(motorH);
  Serial.print(",");
  //Stepper 2
  printStepperInfo(motorV);
  //End list
  Serial.print("]");
  Serial.print("}");
  
  }

void printStepperInfo(Motor motor) {
  Serial.print("{");
  Serial.print("\"angle\":"); 
  Serial.print(motor.angle); 
  Serial.print(","); 
  Serial.print("\"steps\":"); 
  Serial.print(motor.steps); 
  Serial.print(","); 
  Serial.print("\"steps_per_round\":"); 
  Serial.print(steps_per_round); 
  Serial.print(","); 
  Serial.print("\"value\":"); 
  Serial.print(motor.value); 
  Serial.print("}");  
}

void serialEvent(){
  //Recepción de datos Seriales
  
  while (Serial.available()) {              //Si existen datos seriales, leer a todos
    char CaracterEntrada = Serial.read();   //Leer 1 byte serial recibido
   
    cadenaCharEntrada += CaracterEntrada;   //Agregar el nuevo char a una cadena String 
    if (CaracterEntrada == '\n') {          //Si el char o byte recibido es un fin de linea, activa la bandera
      finCadena = true;                        //Si la bandera finCadena = 1, entonces la transmision esta completa
    } 
  }
}
