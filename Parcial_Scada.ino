//Bibliotecas necessarias para exibição no display Grove Light (Wire e rgb_lcd) e protocolo de comunicação de dados (SimpleModbusSlave)
#include <Wire.h>
#include <rgb_lcd.h>
#include <SimpleModbusSlave.h>

// Registradores e seus endereços. OFFSET do registrador relacionado ao potenciometro é o 0, sensor 1, botao 2.
enum 
{     
  // just add or remove registers and your good to go...
  // The first register starts at address 0
  POTENCIOMETRO_VAL,     
  SENSOR_VAL,
  CHAVE_VAL,  
  HOLDING_REGS_SIZE // leave this one
  // total number of registers for function 3 and 16 share the same register array
  // i.e. the same address space
};

// Portas onde os componentes sao conectados ao Arduino.
const int PUSHBTN = 13;
const int POTEN = A2;
const int SENSOR = A3;
bool Statebtn = LOW;
unsigned int holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array


rgb_lcd lcd;
//Instancia e define a cor do LCD.
const int colorR = 255;
const int colorG = 255;
const int colorB = 255;
int Valor = 0;

void setup() 
{
  lcd.begin(16, 2);
    
  lcd.setRGB(colorR, colorG, colorB);
    
  lcd.print("Tensao no sensor");
  modbus_configure(&Serial, 9600, SERIAL_8N1, 1, 2, HOLDING_REGS_SIZE, holdingRegs);
  modbus_update_comms(9600, SERIAL_8N1, 1);
  
  pinMode(PUSHBTN, INPUT);
  
  delay(1000);
}

void loop()
{
  // Trata o apertar do botao com sua correspondente troca de estado.
  if(digitalRead(PUSHBTN) == HIGH){
      Statebtn = !Statebtn;}

  //Conexao dos dados e feita pelo modbus.
  modbus_update();

  //Leitura dos dados pelas portas.
  holdingRegs[SENSOR_VAL] = analogRead(A3);
  holdingRegs[POTENCIOMETRO_VAL] = analogRead(A2);
  holdingRegs[CHAVE_VAL] = Statebtn;
 
// set the cursor to column 10, line 1
 lcd.setCursor(10, 1);

 Valor = analogRead(A3);
 lcd.print(Valor);

 delay(300);
}

