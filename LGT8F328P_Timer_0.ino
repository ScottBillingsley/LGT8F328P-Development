/*
 *            Example of setting up a PWM wave on Timer 0
 *            Vernon Billingsley c2021     
 */

/* Include to change the prescaler frequency on LGT8F board */
#include <avr/power.h>



/************************* Defines ********************************/
#define DEBUG 1

#if DEBUG == 1
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.println( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/************************** Variables *****************************/
/*256 sample sinewave */
const byte sine256[256] PROGMEM = {
  127, 130, 133, 136, 139, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 178, 181, 184, 187, 190, 192, 195, 198, 200, 203, 205, 208, 210, 212, 215,
  217, 219, 221, 223, 225, 227, 229, 231, 233, 234, 236, 238, 239, 240, 242, 243, 244, 245, 247, 248, 249, 249, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254,
  254, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 249, 249, 248, 247, 245, 244, 243, 242, 240, 239, 238, 236, 234, 233, 231, 229, 227, 225, 223, 221, 219,
  217, 215, 212, 210, 208, 205, 203, 200, 198, 195, 192, 190, 187, 184, 181, 178, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 139, 136, 133, 130,
  127, 124, 121, 118, 115, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39,
  37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 16, 15, 14, 12, 11, 10, 9, 7, 6, 5, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11, 12, 14, 15, 16, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,
  37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 115, 118, 121, 124,
};

volatile byte count;

/**************************  Functions ****************************/


/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  /* Set the prescaler to 1 on the LGT8F board for 32 mHz clock */
  clock_prescale_set(clock_div_1);


  
  
  if (DEBUG) {
    Serial.begin(115200);
  }

  /************************* Setup Pins ***************************/
  DDRD |= _BV (2);  /*Test on pin D2 */

  /*PWM output pin D6 */
  DDRD |= _BV (6);

  
  /*************************  Setup Timer0 ************************/
  cli();                //stop interrupts
  
  TCCR0A = 0; /*Clear TCCR0A register */
  TCCR0B = 0; /*Clear TCCR0B register */
  TCNT0  = 0; /*Initialize counter value to 0 */
  /* High Speed Clock sys * 2 */
  /* 125 kHz, 124940 measured */
  sbi(TCKCSR, F2XEN);
  __asm__ __volatile__ ("nop\n\t");
   
  /* Set compare register */
  OCR0A = 255;

  /* Clear pin on up count, set on down */
  cbi(TCCR0A, COM0A0);
  sbi(TCCR0A, COM0A1);
  // Fast PWM
  cbi(TCCR0B, WGM02);
  sbi(TCCR0A, WGM01);
  sbi(TCCR0A, WGM00);
  /*Set prescaler to 1 */
  cbi(TCCR0B, CS02);
  cbi(TCCR0B, CS01);
  sbi(TCCR0B, CS00);
  /* enable timer compare interrupt */
  //sbi(TIMSK0, OCIE0A);
  /* enable timer over flow interrupt */
  sbi(TIMSK0, TOIE0);
  sei();                //allow interrupts


}/**************************  End Setup **************************/
ISR(TIMER0_OVF_vect) {
//ISR(TIMER0_COMPA_vect) {  
  /*Set the compare to the new value */
  OCR0A = pgm_read_byte_near(&(sine256[count]));
  count ++;
  /*Toggle Pin 2 to read accurate clock speed */
  PIND = PIND | 0b00000100;
}

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {


}/*************************** End Loop *****************************/
