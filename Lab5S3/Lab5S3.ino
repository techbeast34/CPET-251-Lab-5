// Lab5_ISR_INT0
// Written by: Nov 01, 2014, JTSchueckler
// revised:  Mar 05, 2017, Clark Hochgraf
// Desc: ++ PLEASE READ ALL COMMENTS ++
//       This program demonstrates a simple use of the INTO functionality.     
//       Review the initialization of the interrupt system.
//       Review the setup of the ISR.
// Define volatile global variables for ISR system interface.
volatile long pulseCount = 0, prevpulseCount = -1;
volatile boolean PCINT1_was_triggered = false;

static byte prevPINC;
byte newPINC;
byte changeMap;

//***********************************************************************************
void setup() {    
  Serial.begin(9600);   Serial.println("Lab 5 ISR INT0 counter");
  
  configurePins();
  // Display the bootup values of EICRA, EIFR and EIMSK
  Serial.print("PCIFR \t"); printlnBinaryByte(PCIFR);
  Serial.print("PCICR \t"); printlnBinaryByte(PCICR);
  Serial.print("PCMSK1 \t"); printlnBinaryByte(PCMSK1);
  Serial.println();
  
  initInterrupts();
// Display the programmed values of EICRA, EIFR and EIMSK
  Serial.print("PCIFR \t"); printlnBinaryByte(PCIFR);
  Serial.print("PCICR \t"); printlnBinaryByte(PCICR);
  Serial.print("PCMSK1 \t"); printlnBinaryByte(PCMSK1);
  Serial.println();
}

//***********************************************************************************
void loop() { 
  cli();
  if (PCINT1_was_triggered){
    Serial.println("PCINT1 was triggered");
    PCINT1_was_triggered=false;
    Serial.println(pulseCount);
  }
  
  sei();
} // Arduino loop()

//===================================================================================
void configurePins(void) {
  pinMode(A5,INPUT_PULLUP);//Set up A0 (INT0) as an INPUT w/Pullup;
  pinMode(A0,INPUT_PULLUP);
}

//===================================================================================
void initInterrupts(void)   {
/* disable interrupts using cli(); and enable interrupts using sei(); with Arduino IDE   
   Typical Atmel C code would use  __disable_interrupt(); 
   to clear the global interrupt bit in the SREG. 
      and  __enable_interrupt(); to set the global interrupt enable in the SREG.
   OR by direct manipulation of the "I" bit in the SREG.

   NOTE: The Serial Monitor and other Arduino functions will be directly affected
   if you try to manipulate the SREG in the Arduino operation. */
  cli(); // Clear the Global INT bit to disable ALL interrrupts

  PCICR |= 0x02;
  PCIFR |= 0x02;
  PCMSK1 |= 0x21;

  
  sei();   // Sets the Global INT bit to enable ALL interrrupts
}
//===================================================================================
void printlnBinaryByte(byte value) { // prints 8-bit data in binary with leading 0's  
  Serial.print("B");
  for (byte bitmask = 0x80; bitmask; bitmask >>= 1) {
        Serial.print((bitmask & value) ? '1' : '0');
    }
  Serial.println();
}
//===================================================================================
ISR(PCINT1_vect){ // the ISR name must match the interrupt (e.g. INT0_vect)
  newPINC = PINC;
  changeMap = newPINC ^ prevPINC;
  if(changeMap & 0x01) pulseCount++;
  if(changeMap & 0x20) pulseCount--;
  PCINT1_was_triggered = true;
}
