#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

#define IN5 8
#define IN6 9
#define IN7 10
#define IN8 11

const int HORIZONTAL = 0;
const int VERTICAL = 1;

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

const int analogPin = A0;

int stepsH = 0; // Define el paso actual de la secuencia
int stepsV = 0; // Define el paso actual de la secuencia
float angleH = 0.0;
float angleV = 0.0;
int valueH; //variable que almacena la lectura analógica raw
int valueV; //variable que almacena la lectura analógica raw
int position; //posicion del potenciometro en tanto por ciento
float delayValueH = 0;
float delayValueV = 0;

float steps_per_round = (64*64);
float angle_paso = 360.0/steps_per_round;
String cadenaCharEntrada = "";  
char vectorChar[20];
bool finCadena = false;


void setup()
{
  Serial.begin(9600); 
  //HORIZONTAL STEPPER 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //VERTICAL STEPPER
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  
  cadenaCharEntrada.reserve(20);
}

void loop()
{
  delayValueH = 0;
  delayValueV = 0;  
  
  if(finCadena) {                               //Se termino de recibir una cadena completa
    
    finCadena = false;                                  //Permitimos volver a recibir otra cadena
    cadenaCharEntrada.toCharArray(vectorChar,20);   //Convertimos el string a un vectorChar
    cadenaCharEntrada = "";
    int numeroEntero = atoi(vectorChar);            //Convertimos un vectorChar a un enter 
    int type = HORIZONTAL;
    if(numeroEntero == 4 || numeroEntero == 5 || numeroEntero == 6) {
      type = VERTICAL;
    }
    if(numeroEntero == 1 || numeroEntero == 4){
      position = 99;
      for(int i = 0; i < 10; i++) {    
        stepper(type); // Avanza un paso
        delay (10) ; //Un paso cada 250 ms para ver los leds
      }
      return;
    }
    else if(numeroEntero == 2 || numeroEntero == 5){
      position = 1;
      for(int i = 0; i < 10; i++) {    
        stepper(type); // Avanza un paso
        delay (10) ; //Un paso cada 250 ms para ver los leds
      }
      return;
    }
    else if(numeroEntero == 3 || numeroEntero == 6){
      float *angle = 0;
      int *steps = 0;
      int *value = 0;
      if(numeroEntero == 3) {
        angle = &angleH;
        steps = &stepsH;
        value = &valueH;
      } else if (numeroEntero == 6) {
        angle = &angleV;
        steps = &stepsV;
        value = &valueV;
      }   
      Serial.print("angle="); 
      Serial.print(*angle); 
      Serial.print(";"); 
      Serial.print("steps="); 
      Serial.print(*steps); 
      Serial.print(";"); 
      Serial.print("steps_per_round="); 
      Serial.print(steps_per_round); 
      Serial.print(";"); 
      Serial.print("value="); 
      Serial.print(*value); 
      Serial.print(";"); 
      Serial.print("angle="); 
      Serial.print(*angle); 
      Serial.print(";"); 
      Serial.print("bobinas=");
      Serial.print(Paso[*steps % 8][0]);
      Serial.print(Paso[*steps % 8][1]);
      Serial.print(Paso[*steps % 8][2]);
      Serial.print(Paso[*steps % 8][3]);  
      delay (10) ;
    }
  }
  //Descomentar para usar potenciometro
  /*  
  else {  
      value = analogRead(analogPin); // realizar la lectura analógica raw
      position = map(value, 0, 1023, 1, 99); // convertir a porcentaje
  }

  if(position > 55 || position < 45) {      
    if(position == 0) {
      position = 1;      
    }
    else if(position == 100) {
      position = 99;
    }
    delayValue = 100/(float)(abs((position - 50)));     
    stepper(); // Avanza un paso
    delay (delayValue) ; //Un paso cada 250 ms para ver los leds
  }
  */ 
}


void stepper(int type) //Avanza un paso                                                                                                                                 
{
  int *steps = 0;
  float *angle = 0; 
  
  if(type == HORIZONTAL) {
    steps = &stepsH;
    angle = &angleH; 
    digitalWrite( IN1, Paso[*steps % 8][ 0] );  
    digitalWrite( IN2, Paso[*steps % 8][ 1] );  
    digitalWrite( IN3, Paso[*steps % 8][ 2] );  
    digitalWrite( IN4, Paso[*steps % 8][ 3] );
  } else {
    steps = &stepsV;
    angle = &angleV; 
    digitalWrite( IN5, Paso[*steps % 8][ 0] );  
    digitalWrite( IN6, Paso[*steps % 8][ 1] );  
    digitalWrite( IN7, Paso[*steps % 8][ 2] );  
    digitalWrite( IN8, Paso[*steps % 8][ 3] );
  }
  
  if (position > 50){  
    *steps--;
    *angle -= angle_paso; 
  }
  else {
    *steps++;
    *angle += angle_paso;
  }

  if(*angle < 0.0) {
    *angle +=360.0;
  } 
  if(*angle > 360.0) {
    *angle -= 360.0;
  }
  if(*steps < 0.0) {
    *steps += steps_per_round;
  } 
  if(*steps > steps_per_round) {
    *steps -= steps_per_round;
  }
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
