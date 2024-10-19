//
// GPIO_Keypad : 3x3 keypad input and control LEDs (or Relays)
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN

// PA0,1,2,3,4,5 connected to 3x3 Keypad
// PC12,13,14,15 connected to LEDs (or Relays)

#include <stdio.h>
#include <stdlib.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Scankey.h"
#include "LCD.h"


int main(void)
{
	uint32_t state =0;
	uint32_t i =0;
	uint32_t last_i = 0;
	uint32_t numberA = 0;
	uint32_t numberB = 0;
	SYS_Init();
	OpenKeyPad();
	init_LCD();
	clear_LCD();
	char Text[16] = "               ";
 	while(1) 
  {
		i=ScanKey();
		if(i!=0){
				last_i=i;
				continue;
		}
		if(last_i>=1&&last_i<=9){
				if(state == 0){
						numberA = last_i;
						state = 1;
				}
				if(state == 1){
						numberB = last_i;
						sprintf(Text,"%d + %d  =  %d",numberA,numberB,numberA+numberB);	  
						print_Line(0, Text);
						sprintf(Text,"%d - %d  =  %d",numberA,numberB,numberA-numberB);	  
						print_Line(1, Text);
						sprintf(Text,"%d * %d  =  %d",numberA,numberB,numberA*numberB);	  
						print_Line(2, Text);
						sprintf(Text,"%d / %d  =  %d",numberA,numberB,numberA/numberB);	  
						print_Line(3, Text);
						state = 2;
				}
				if(state == 2){
						clear_LCD();
						state = 0;
				}
				last_i = 0;
		}
		CLK_SysTickDelay(5000); 																 
	}
}


