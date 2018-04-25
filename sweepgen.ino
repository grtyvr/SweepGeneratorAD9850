/* Based on AD9851 code from Andrew Smallbone - modified for AD9850
   http://www.rocketnumbernine.com/2011/10/25/programming-the-ad9851-dds-synthesizer 
 */

#define AD9850_CLOCK 125000000         // Module crystal frequency. Tweak here for accuracy.

#define W_CLK 8                        // AD9850 Module pins.    
#define FQ_UD 7       
#define DATA  6       
#define RESET 5     

long unsigned int freq = 1000;         // Set initial frequency.

#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }


// transfers a byte, a bit at a time, LSB first to the 9850 via serial DATA line
void tfr_byte(byte data) {
  for (int i = 0; i < 8; i++, data >>= 1) { // shift off bits after we send them
    digitalWrite(DATA, data & 0x01);        // mask off all but the LSB
    pulseHigh(W_CLK);                       //after each bit sent, CLK is pulsed high
  }
}

void sendFrequency(double frequency) {
  int32_t freq1 = frequency * 4294967295/AD9850_CLOCK;  // note 125 MHz clock on 9850
  for (int b = 0; b < 4; b++, freq1 >>= 8) {
    tfr_byte(freq1 & 0xFF);
  }
  tfr_byte(0x000);                     // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);                    // Done!  Should see output
}


void setup() {


  
  pinMode(FQ_UD, OUTPUT);              // Configure pins for output to AD9850 module.
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
  

  // Initialise the AD9850 module. 
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);    // this pulse enables serial mode - Datasheet page 12 figure 10  
  
}

void loop() {
// for testing just loop between 100HZ and 1KHz in 1 Hz steps
  for ( freq = 100; freq < 10000; freq++ ){
    sendFrequency(freq);
  }

}
