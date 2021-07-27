/*
 *            Example of setting up ADC
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

  /*************************  Setup ADC ***************************/
  /*Set to Right Adjust for 4096 precision */
  cbi(ADMUX, ADLAR);

  /*Set to VRef to AVCC */
  cbi(ADCSRD, REFS2);
  cbi(ADMUX, REFS1);
  sbi(ADMUX, REFS0);

  /*Set to ADC0 to start */
  cbi(ADMUX, MUX3);
  cbi(ADMUX, MUX2);
  cbi(ADMUX, MUX1);
  cbi(ADMUX, MUX0);

  /*Set prescaler to 32 */
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  /*No detuning compensate */
  cbi(ADCSRC, OFEN);
  /*Clear polarity control for normal operation */
  cbi(ADCSRC, SPN);
  /*Diable auto channel monitoring */
  cbi(ADCSRC, AMEN);
  /*Enable HIGH speed for low resistance load */
  sbi(ADCSRC, SPD);
  /*Conversion from multiplexer */
  cbi(ADCSRC, DIFS);
  /*Disable digital control on A0, PC0D */
  sbi(DIDR0, PC0D);
  /*Disable digital control on A1, PC1D */
  //sbi(DIDR0, PC1D);
  /*Disable digital control on A2, PC2D */
  //sbi(DIDR0, PC2D); 
  /*Disable digital control on A3, PC3D */
  //sbi(DIDR0, PC3D); 
  /*Disable digital control on A4, PC4D */
  //sbi(DIDR0, PC4D);
  /*Disable digital control on A5, PC5D */
  //sbi(DIDR0, PC5D);
  /*Disable digital control on A6, PE1D */
  //sbi(DIDR0, PE1D);
  /*Disable digital control on A7, PE3D */
  //sbi(DIDR0, PE3D);      
  
  /*Reference volatage to 4.096 */
  sbi(ADCSRD, IVSEL);

  /*Turn off Auto Trigger */
  cbi(ADCSRA, ADATE);

  /*Turn the ADC ON  */
  sbi(ADCSRA, ADEN);

  /*Start the first conversion */
  sbi(ADCSRA, ADSC);

  
}/**************************  End Setup **************************/


/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  if (!(bitRead(ADCSRA, ADSC))) {
    /*Read and store the results  */
    uint8_t temp_adcl = ADCL;
    uint16_t temp_adc = (ADCH << 8) + temp_adcl;
    Serial.println(temp_adc >> 2);
    /*Start the next conversion */
    sbi(ADCSRA, ADSC);
  }

   delay(100); 

}/*************************** End Loop *****************************/
