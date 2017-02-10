#include <SimpleModbusSlave.h>

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

const int PUSHBTN = 13;
const int POTEN = A2;
const int SENSOR = A0;
bool Statebtn = LOW;
unsigned int holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array
////////////////////////////////////////////////////////////

void setup()
{
  /* parameters(HardwareSerial* SerialPort,
                long baudrate, 
		unsigned char byteFormat,
                unsigned char ID, 
                unsigned char transmit enable pin, 
                unsigned int holding registers size,
                unsigned int* holding register array)
  */
  
  /* Valid modbus byte formats are:
     SERIAL_8N2: 1 start bit, 8 data bits, 2 stop bits
     SERIAL_8E1: 1 start bit, 8 data bits, 1 Even parity bit, 1 stop bit
     SERIAL_8O1: 1 start bit, 8 data bits, 1 Odd parity bit, 1 stop bit
     
     You can obviously use SERIAL_8N1 but this does not adhere to the
     Modbus specifications. That said, I have tested the SERIAL_8N1 option 
     on various commercial masters and slaves that were suppose to adhere
     to this specification and was always able to communicate... Go figure.
     
     These byte formats are already defined in the Arduino global name space. 
  */
	
  modbus_configure(&Serial, 9600, SERIAL_8N1, 1, 2, HOLDING_REGS_SIZE, holdingRegs);

  // modbus_update_comms(baud, byteFormat, id) is not needed but allows for easy update of the
  // port variables and slave id dynamically in any function.
  modbus_update_comms(9600, SERIAL_8N1, 1);
  
  pinMode(PUSHBTN, INPUT);
}

void loop()
{
  if(PUSHBTN == HIGH)
      Statebtn = !Statebtn;
  
  modbus_update();
  
  holdingRegs[SENSOR_VAL] = analogRead(SENSOR);
  holdingRegs[POTENCIOMETRO_VAL] = analogRead(POTEN);
  holdingRegs[POTENCIOMETRO_VAL] = digitalRead(Statebtn);
}

