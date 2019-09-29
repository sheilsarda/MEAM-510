/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"//include the usb headerfile
#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10);m_usb_tx_char(13); //Print number x and start off a new line
#define PRINTSTRING(x) m_usb_tx_string(x); m_usb_tx_char(10); m_usb_tx_char(13);//Print string x and start off a new line

int xflag = 0;//set a flag when x should be the short pulse
int synccount = 0;//set a counter for number of synctimes
int yflag = 0;//set a flag when y should be the short pulse 
unsigned int startofpulse, endofpulse, pulsewidth;
unsigned int lastY, lastX, lastsync;

int main(void)
{
    m_usb_init();
    set(TCCR3B,ICES3);
    set(TCCR3B,CS31);
    set(TCCR3B,CS30);

    teensy_wait(200);
    PRINTSTRING("Please wait for started");

    for(;;)
    {

      while (!bit_is_set(TIFR3,ICF3));
      startofpulse = ICR3;
      set(TIFR3,ICF3);
      toggle(TCCR3B,ICES3);
      while (!bit_is_set(TIFR3,ICF3));
      endofpulse = ICR3;
      pulsewidth = endofpulse - startofpulse;
      set(TIFR3, ICF3);
      toggle(TCCR3B,ICES3);
      if (pulsewidth >= 12 && pulsewidth < 68)
      {
        // PRINTNUM(1);
        synccount = synccount + 1;
        if (synccount > 1 && yflag == 1)
        {
          yflag = 0;
        }
        if (synccount == 3)
        {
          xflag = 1;
        }
        if (synccount == 4)
        {
          synccount = 0;
          xflag = 0;
        }
        }
      lastsync = startofpulse;



      if (pulsewidth > 2 && pulsewidth < 6)
      {
         // PRINTNUM(0);
          synccount = 0;
          if (xflag == 1)
          {
            lastX = startofpulse - lastsync;
            yflag = 1;
            xflag = 0;

          }
          if (yflag == 1)
          {
            lastY = startofpulse - lastsync;
            yflag = 0;

          }

      }
      PRINTSTRING("The time between last X Y pulse and Sync pulse is:");
      m_usb_tx_uint(lastX);
      m_usb_tx_char(3);
      PRINTNUM(lastY);
    }



    return 0;   /* never reached */
}
