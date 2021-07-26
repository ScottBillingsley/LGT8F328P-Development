/*
 *            Example of setting up CTC on Timer 2
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



  
  /*************************  Setup Timer2 ************************/
  cli();                //stop interrupts
  
  TCCR2A = 0; /*Clear TCCR2A register */
  TCCR2B = 0; /*Clear TCCR2B register */
  TCNT2  = 0; /*Initialize counter value to 0 */
  
  /* Set compare register */
  /* (32,000,000 / ( 8 * 16384)) - 1            */
  /* Calculated 16384 kHz, measured 16387.5 kHz */
  OCR2A = 243;

  /* CTC OCR2A */
  cbi(TCCR2B, WGM22);
  sbi(TCCR2A, WGM21);
  cbi(TCCR2A, WGM20);
  
  /*Set prescaler to /8 */
  cbi(TCCR2B, CS22);
  sbi(TCCR2B, CS21);
  cbi(TCCR2B, CS20);
  
  /* enable timer compare interrupt */
  sbi(TIMSK2, OCIE2A);

  sei();                //allow interrupts


}/**************************  End Setup **************************/
ISR(TIMER2_COMPA_vect) {  

  /*Toggle Pin 2 to read accurate clock speed */
  PIND = PIND | 0b00000100;
}

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {


}/*************************** End Loop *****************************/
