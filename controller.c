/**
 *  Setup / Config
 */
#DEFINE BT_SETUP_PIN  PIN_A3
#DEFINE BT_START_PIN  PIN_A4

/**
 * Global Var
 */
int setup_mode = 0;


/**
 * Function that initialize the buttons
 */
void bt_init() {
   output_float(BT_START_PIN);
   output_float(BT_SETUP_PIN);
}


/**
 * Function that controls the START button
 */
void bt_start() {
   static boolean bt_press = false;
   
   // Button pressed
   if (!input(BT_START_PIN)) {
      bt_press = true;
      disp_delay_10ms(2);
   }
   else {
      
      // Button Released
      if (bt_press) {
         bt_press = false;
         
         // Adding the remaining time
         timer_set_min_remains(min_remains + min_credits);
         
         // Adding 1 second to the remaining counting time
         if (!flag_timer_start && min_remains != 0 && sec_remains == 0) sec_remains++;
         
         // Start the conting time
         flag_timer_start = true;
      }
   }
}


/**
 * Function that controls the SETUP button
 */
void bt_setup() {
   static boolean bt_press = false;
   
   // Button pressed
   if (!input(BT_SETUP_PIN)) {
      bt_press = true;
      disp_delay_10ms(2);
   }
   else {
      
      // Button Released
      if (bt_press) {
         bt_press = false;
         
         // Enabling the Setup mode and changing the Steps
         setup_mode++;
      }
   }
}


/**
 * Function that controls the Config mode
 */
void setup() {
   static int uni = 0, dez = 0;
   boolean flag_timer_temp;
   
   switch (setup_mode) {
      
      // Displaying the credit value by minute flashing into display
      case 1:
         
         // Reading the setup minutes
         dez = min_credits / 10;
         uni = min_credits % 10;
         
         // Showing the values flashing
         flag_disp1 = flag_timer_1sec;
         flag_disp2 = flag_disp1;
         break;
      
	  // Displaying the tens flashing and increasing by each flash
      case 2 :
         
         // Displaing the tens flashing and increasing
         flag_disp1 = true;
         flag_timer_temp = flag_timer_1sec;
         if (!flag_disp2 && flag_disp2 != flag_timer_temp) {
            dez++;
            dez = dez % 10;
         }
         flag_disp2 = flag_timer_temp;
         break;
      
	  // Displaying the units flashing and increasing by each flash
      case 3 :
         
         // Displaying the units flashing and increasing
         flag_disp2 = true;
         flag_timer_temp = flag_timer_1sec;
         if (!flag_disp1 && flag_disp1 != flag_timer_temp) {
            uni++;
            uni = uni % 10;
         }
         flag_disp1 = flag_timer_temp;
         break;
      
      // Storing into EEPROM memory
      default :
         
         // Enabling the units and tens
         disp_turn_on();
         
         // Storing the chosen value
         min_credits = (dez * 10) + uni;
         mem_save_min_credits();
         setup_mode = 0;
         
         // Displaying the final result
         disp_output(uni, dez);
         disp_delay_10ms(100);
         disp_turn_off();
         disp_delay_10ms(50);
         return;
   }
   
   // Displaying the minutes by each credit
   disp_output(uni, dez);
}
