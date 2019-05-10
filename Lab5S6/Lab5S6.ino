volatile int pulseCount = 0;
volatile boolean interruptTriggered = false;

void setup() {
  TCCR1A |= 0x83;
  TCCR1B &= 0xF0;
  TCCR1B |= 0x1D;
  
  OCR1A = 15625;
  Serial.begin(9600);

  
  cli();
  TIMSK1 |= 0x02;
  TIFR1 |= 0x02;
  
  sei();
  Serial.println("Timer1 program was started");
  TCNT1 = 0;
  
  
}

void loop() {
  if(interruptTriggered){
    Serial.print("Current count is: ");
    Serial.println(pulseCount);
    interruptTriggered = false;
    TCNT1 = 0;
  }
}

ISR(TIMER1_COMPA_vect){
  pulseCount++;
  interruptTriggered = true;
}
