/**
 *  Setup / Config
 */
#DEFINE LED_SECS_PIN  PIN_A7
#DEFINE BUZZER_PIN    PIN_A2
#DEFINE RELE_PIN      PIN_A6

/**
 * Global Var
 */
boolean flag_timer_1sec = false, flag_timer_start = false;
int min_remains = 0, sec_remains = 0, min_credits = 0;


/**
 * Function that define the time in minutes without the interruption of TIMER1
 */
void timer_set_min_remains(int min) {
   boolean flag_temp;
   
   // Disabling the timer
   flag_temp = flag_timer_start;
   flag_timer_start = false;
   
   // Defining the minutes
   min_remains = min;
   
   // Enabling the timer
   flag_timer_start = flag_temp;
}


/**
 * Function that send to the display the current remaining time
 */
void timer_display() {
   // Displaying the seconds
   if (min_remains == 0) {
      // Enabling the display
      if (sec_remains == 0) {
         disp_turn_on();
         flag_timer_start = false;
      }
      // Showing the seconds flashing
      else {
         flag_disp1 = flag_timer_1sec;
         flag_disp2 = flag_disp1;
      }
      disp_time(sec_remains);
   }
   // Showing the minutes/hours
   else {
      // Enabling the display
      disp_turn_on();
      disp_time(min_remains);
   }
}


/**
 * Function that initialize the TIMER1 to count 500ms
 */
void timer_init() {
   // Setup the A port
   set_tris_a(0x18);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   set_timer1(0x0BDB);
   enable_interrupts(INT_TIMER1);
}


/**
 * Function that decreases the regressive timer values
 */
#INT_TIMER1
void timer_int_timer1() {
   
   // Defining the timer1 to count 500ms
   set_timer1(0x0BDB + get_timer1());
   
   // Invert the flag to count 500ms
   flag_timer_1sec = !flag_timer_1sec;
   
   // Decreasing 1 second
   if (flag_timer_start) {
      if (flag_timer_1sec) {
         if (sec_remains == 0) {
            if (min_remains > 0) {
               min_remains--;
               sec_remains = 59;
            }
         }
         else sec_remains--;
      }
      
      // Showing the seconds Led flashing
      output_bit(LED_SECS_PIN, flag_timer_1sec);
      
      // Ringing the buzzer at the final minute
      if (min_remains == 0) output_bit(BUZZER_PIN, flag_timer_1sec);
      else output_low(BUZZER_PIN);
      
      // Enabling the relay output
      output_high(RELE_PIN);
   }
   else {
      // Stoping the Led and the Buzzer
      output_low(LED_SECS_PIN);
      output_low(BUZZER_PIN);
      output_low(RELE_PIN);
   }
}
