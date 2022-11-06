/*
                   LGT8F328P Comparator Test  
                   Vernon Billingsley c2022

                   Set up the analog comparator on 
                  pins D6 and D7 to control an LED
                  on pin D13..

 
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission
    notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

                   

                       LGT8F328P nano
                     -----------------
        TXD PD1   --| D1              |-- Vin
        RXD PD0   --| D0              |-- GND
        Rest      --|                 |-- PC6 Reset
        GND       --|                 |-- 5 volt
        INT0 PD2  --| D2              |-- ADC7
   OC2B INT1 PD3  --| D3              |-- ADC6
             PD4  --| D4         ADC5 |-- PC5 ADC5 SCL
   OC0B      PD5  --| D5         ADC4 |-- PC4 ADC4 SDA
  OC0A +AIN0 PD6  --| D6         ADC3 |-- PC3 ADC3
       -AIN1 PD7  --| D7         ADC2 |-- PC2 ADC2
        CLK0 PB0  --| D8         ADC1 |-- PC1 ADC1
        OC1A PB1  --| D9         ADC0 |-- PC0 ADC0
   SS   OC1B PB2  --| D10        AREF |--
   MOSI OC2A PB3  --| D11             |-- 3.3 volt
   MISO      PB4  --| D12         D13 |-- PB5 SCK LED
                      ----------------

                 Pins:
                 Nano    Function
                 0
                 1
                 2
                 3
                 4
                 5
                 6      + Input
                 7      - Input
                 8
                 9
                 10
                 11
                 12
                 13     Output
                 A0
                 A1
                 A2
                 A3
                 A4
                 A5
                 A6
                 A7

*/

/* Include to change the prescaler frequency on LGT8F board */
#include <avr/power.h>



/************************* Defines ********************************/
#define DEBUG 0

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
  DDRB |= _BV (5); // pinMode (13, OUTPUT);

  /************************* Setup Pins ***************************/
  DDRB |= _BV (5); // pinMode (13, OUTPUT);


  /*Setup the comparator */
  cbi(DDRD, PD6); /*AIN0 + NANO D6*/
  cbi(DDRD, PD7); /*AIN1 - NAN0 D7*/
  /*Disable the digital input buffer */
  sbi(DIDR1, PE6D);
  sbi(DIDR1, PE7D);
  /* Open the comparator */
  cbi(C0SR, C0D);
  /* Set the positive input to D6 */
  cbi(C0SR, C0BG);
  cbi(C0XR, C0PS0);
  /* Set the interrupt */
  sbi(C0SR, C0IE);
  /* Select toggle */
  cbi(C0SR, C0IS1);
  cbi(C0SR, C0IS0);
  /* Set negative input to D7 */
  cbi(ADCSRB, 6);
  cbi(ADCSRB, 7);

  sei();                //allow interrupts

}/**************************  End Setup **************************/

ISR(ANALOG_COMP_vect) {
  /* Read the state of the comparator output */
  if ((C0SR >> 5) & 0x01 == 1) {
    PORTB |= _BV (5); // digitalWrite (13, HIGH);
  } else {
    PORTB &= ~_BV (5); // digitalWrite (13, LOW);
  }
}

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {



}/*************************** End Loop *****************************/
