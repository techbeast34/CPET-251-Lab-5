// Lab5_ISR_INT0
// Written by: Nov 01, 2014, JTSchueckler
// revised:  Mar 05, 2017, Clark Hochgraf
// Desc: ++ PLEASE READ ALL COMMENTS ++
//       This program demonstrates a simple use of the INTO functionality.     
//       Review the initialization of the interrupt system.
//       Review the setup of the ISR.
// Define volatile global variables for ISR system interface.
volatile long pulseCount = 0, prevpulseCount = -1;

//***********************************************************************************
void setup() {    
  Serial.begin(9600);   Serial.println("Lab 5 ISR INT0 counter");
  
  configurePins();
  // Display the bootup values of EICRA, EIFR and EIMSK
  Serial.print("EIFR \t"); printlnBinaryByte(EIFR);
  Serial.print("EICRA \t"); printlnBinaryByte(EICRA);
  Serial.print("EIMSK \t"); printlnBinaryByte(EIMSK);
  Serial.println();
  
  initInterrupts();
// Display the programmed values of EICRA, EIFR and EIMSK
  Serial.print("EIFR \t"); printlnBinaryByte(EIFR);
  Serial.print("EICRA \t"); printlnBinaryByte(EICRA);
  Serial.print("EIMSK \t"); printlnBinaryByte(EIMSK);
  Serial.println();
}

//***********************************************************************************
void loop() { 
  cli();
  if (pulseCount != prevpulseCount)  { // only print the pulse count if it has changed
    prevpulseCount = pulseCount;
    Serial.print("Switch was pressed ");
    Serial.print(pulseCount);
    Serial.println(" times.");
  }
  sei();
} // Arduino loop()

//===================================================================================
void configurePins(void) {
  pinMode(2,INPUT_PULLUP); //Set up PD2 (INT0) as an INPUT w/Pullup;
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
  EICRA |=(1<<1); // Configure EICRA to detect falling edge for INT0 
         // ISC0 = 0b10 refers to Interrupt Sense Control for INT0
         // the two bits {1 and 0} program the hardware to respond to a falling edge
         // at the input pin INT0 (PD2)
  EIMSK |= 0x01;  // Unmask INT0 in EIMSK so that it is enabled
  EIFR  |= 0x01; // Clear any pending interrupt flags for INTO by writing a 1 to bit 0
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
ISR(INT0_vect){ // the ISR name must match the interrupt (e.g. INT0_vect)
  pulseCount++;
  // NOTE: NO Extraneous instructions in the ISR
}
