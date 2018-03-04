#include <16F628A.h>
#FUSES NOWDT, INTRC_IO, PUT, NOPROTECT, NOBROWNOUT, MCLR, NOLVP, NOCPD
#USE delay(clock=4000000)

#use fast_io(A)
#use fast_io(B)

// Includes
#include "display_led.c"
#include "timer_1sec.c"
#include "mem_eeprom.c"
#include "controller.c"


/**
 * Main function
 */
void main() {
   
   // Initializing the display and the timer
   disp_init();
   timer_init();
   mem_init();
   bt_init();
   
   // Enabling all interruptions
   enable_interrupts(GLOBAL);
   
   // Showing the test banner on the Display
   disp_banner();
   
   // Initializing the timer counting
   flag_timer_start = true;
   
   // Loop
   for (;;) {
      
	  // Monitoring the actions of the Start / Setup buttons
      bt_start();
      bt_setup();
      
	  // Setup Mode / Regressive Timer Mode
      if (setup_mode != 0) setup();
      else timer_display();
      
	  // Updating the display
      disp_refresh();
      
	  // Saving the minutes by each decreasing
      mem_save_min_remains();
   }
}
