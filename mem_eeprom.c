/**
 *  Setup / Config
 */
#DEFINE MEM_CREDITS_ADDR  0x00
#DEFINE MEM_REMAINS_ADDR  0x01

#ROM  0x2100={0x01, 0x00}

/**
 * Global Var
 */
int mem_min_remains = 0;


/**
 * Function that loads the time by each credit from EEPROM
 */
void mem_load_min_credits() {
   int min;
   min = read_eeprom(MEM_CREDITS_ADDR);
   
   if (min > 99) min = 0;
   min_credits = min;
}


/**
 * Function that stores the time by each credit into EEPROM
 */
void mem_save_min_credits() {
   write_eeprom(MEM_CREDITS_ADDR, min_credits);
}


/**
 * Function that loads the remaining time from EEPROM
 */
void mem_load_min_remains() {
   mem_min_remains = read_eeprom(MEM_REMAINS_ADDR);
   timer_set_min_remains( mem_min_remains );
   if (mem_min_remains != 0) sec_remains = 59;
}


/**
 * Function that stores the remaining time into EEPROM
 */
void mem_save_min_remains() {
   if (mem_min_remains != min_remains) {
      mem_min_remains = min_remains;
      write_eeprom(MEM_REMAINS_ADDR, mem_min_remains);
   }
}


/**
 * Function that initialize the remaining minutes and credits into EEPROM
 */
void mem_init() {
   mem_load_min_credits();
   mem_load_min_remains();
}
