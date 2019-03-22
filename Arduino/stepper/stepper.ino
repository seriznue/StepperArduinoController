#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

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

int steps = 0; // Define el paso actual de la secuencia
float angle = 0.0;
float steps_per_round = (64*64);
float angle_paso = 360.0/steps_per_round;
const int analogPin = A0;
int value; //variable que almacena la lectura analógica raw

int position; //posicion del potenciometro en tanto por ciento
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
  cadenaCharEntrada.reserve(20);
}

void loop()
{
  delayValue = 0; 
  
  if(finCadena) {                               //Se termino de recibir una cadena completa
    
    finCadena = false;                                  //Permitimos volver a recibir otra cadena
    cadenaCharEntrada.toCharArray(vectorChar,20);   //Convertimos el string a un vectorChar
    int numeroEntero = atoi(vectorChar);            //Convertimos un vectorChar a un entero
    
//    Serial.print("Se recibio ");
//    Serial.println(numeroEntero);
   
    if(numeroEntero == 1){
      position = 99;
    }
    else if(numeroEntero == 2){
      position = 1;
    }
    else if(numeroEntero == 3){   
      Serial.print("angle="); 
      Serial.print(angle); 
      Serial.print(";"); 
      Serial.print("steps="); 
      Serial.print(steps); 
      Serial.print(";"); 
      Serial.print("steps_per_round="); 
      Serial.print(steps_per_round); 
      Serial.print(";"); 
      Serial.print("value="); 
      Serial.print(value); 
      Serial.print(";"); 
      Serial.print("angle="); 
      Serial.print(angle); 
      Serial.print(";"); 
      Serial.print("bobinas=");
      Serial.print(Paso[steps % 8][0]);
      Serial.print(Paso[steps % 8][1]);
      Serial.print(Paso[steps % 8][2]);
      Serial.print(Paso[steps % 8][3]);  
    }
    
    cadenaCharEntrada = "";
  } 
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
}


void stepper() //Avanza un paso                                                                                                                                 
{

  digitalWrite( IN1, Paso[steps % 8][ 0] );  
  digitalWrite( IN2, Paso[steps % 8][ 1] );  
  digitalWrite( IN3, Paso[steps % 8][ 2] );  
  digitalWrite( IN4, Paso[steps % 8][ 3] );
  
  if (position > 50){  
    steps--;
    angle -= angle_paso; 
  }
  else {
    steps++;
    angle += angle_paso;
  }

  if(angle < 0.0) {
    angle +=360.0;
  } 
  if(angle > 360.0) {
    angle -= 360.0;
  }
  if(steps < 0.0) {
    steps += steps_per_round;
  } 
  if(steps > steps_per_round) {
    steps -= steps_per_round;
  }

//  Serial.println("Paso: ");
//  
//  Serial.println(steps);
//  
//  Serial.print("Grados desplazado: ");
//  
//  Serial.println(angle);
//  
//  Serial.println("Bobinas activas:\n1 -> ");
//  
//  Serial.println(Paso[steps % 8][0]);
//  
//  Serial.println(Paso[steps % 8][1]);
//  
//  Serial.println(Paso[steps % 8][2]);
//  
//  Serial.println(Paso[steps % 8][3]);
//  
//  Serial.println();
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
