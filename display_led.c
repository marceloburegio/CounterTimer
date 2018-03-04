/**
 *  Setup / Config
 */
#DEFINE DISP1_PIN    PIN_A0
#DEFINE DISP2_PIN    PIN_A1

/**
 * Global Var
 */
boolean flag_disp1 = false, flag_disp2 = false;
int disp1_byte = 0, disp2_byte = 0;


/**
 * Function that enables the display
 */
void disp_turn_on() {
   flag_disp1 = true;
   flag_disp2 = true;
}


/**
 * Function that disables the display
 */
void disp_turn_off() {
   flag_disp1 = false;
   flag_disp2 = false;
}


/**
 * Showing the number passed by parameter in the display
 */
void disp_output(int uni, int dez) {
   
   // Restricting the maximum value
   if (uni > 10) uni = 0;
   if (dez > 10) dez = 0;
   
   // Showing in the display
   disp1_byte = uni;
   disp2_byte = dez;
}


/**
 * Function that initialize the setup of 7-seg display
 */
void disp_init() {
   
   // Defining the level "ZERO" in the transistors
   output_low(DISP1_PIN);
   output_low(DISP2_PIN);
   
   // Setup and initialize the B Port
   set_tris_b(0x00);
   output_b(0x00);
   
   // Initializing the bytes
   disp_output(disp1_byte, disp2_byte);
   
   // Enabling the displays
   disp_turn_on();
}


/**
 * Updating the values in the 7-seg display
 */
void disp_refresh() {
   static boolean flag_disp = false;
   const byte led_map[] = {
      0b11101110, // 0
      0b00101000, // 1
      0b11001101, // 2
      0b01101101, // 3
      0b00101011, // 4
      0b01100111, // 5
      0b11100111, // 6
      0b00101100, // 7
      0b11101111, // 8
      0b01101111, // 9
      0b10101011  // H
   };
   
   // Units
   if (flag_disp) {
      output_low(DISP2_PIN);
      if (flag_disp1) {
         output_b( led_map[ disp1_byte ] );
         output_high(DISP1_PIN);
      }
      else delay_cycles(12);
   }
   // Tens
   else {
      output_low(DISP1_PIN);
      if (flag_disp2) {
         output_b( led_map[ disp2_byte ] );
         output_high(DISP2_PIN);
      }
      else delay_cycles(12);
   }
   flag_disp = !flag_disp;
}


/**
 * Function that performs a delay of 10ms x a factor and showing in the display the result
 * Total Delay: 10ms / 10.000us / 100fps x factor
 */
void disp_delay_10ms(signed int factor) {
   int i;
   
   do {
	  // Updating both the displays
      disp_refresh();                              // Delay: 23us/cycles (Cycles)
      for (i = 0; i < 20; i++) delay_cycles(250);  // Delay: 5.000us/cycles ~ 5ms
      
      disp_refresh();                              // Delay: 23us/cycles (Cycles)
      for (i = 0; i < 20; i++) delay_cycles(250);  // Delay: 5.000us/cycles ~ 5ms
      
      // Decreasing the factor
      factor--;
   } while (factor > 0);
   
   // Updating the display
   disp_refresh();
}


/**
 * Function that shows a banner in the display
 */
void disp_banner() {
   signed int i;
   for (i = 9; i >= 0; i--) {
      disp_output(i, i);
      disp_delay_10ms(15);
   }
   // Final effect that the display flashes the number "00"
   disp_turn_off();
   disp_delay_10ms(30);
   disp_turn_on();
}


/**
 * Showing the time by parameter in the display
 */
void disp_time(int min) {
   int uni, dez;
   
   // Showing only the minutes/seconds
   if (min < 60) {
      dez = min / 10;
      uni = min % 10;
   }
   // Showing only hours
   else {
      dez = min / 60;
      uni = 0x0A;
   }
   
   // Displaying in the display
   disp_output(uni, dez);
}
