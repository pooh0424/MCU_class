//
// GPIO_Keypad : 3x3 keypad input and control LEDs (or Relays)
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN

// PA0,1,2,3,4,5 connected to 3x3 Keypad
// PC12,13,14,15 connected to LEDs (or Relays)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Scankey.h"
volatile uint32_t* leds[4] = {&PC12, &PC13, &PC14, &PC15};

void Init_GPIO(void)
{
	  GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT14, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT15, GPIO_MODE_OUTPUT);
	  PC12=1; PC13=1; PC14=1; PC15=1;
}
void showbinary(uint32_t number){
		for(int i=3;i>=0;i--){
				*(leds[i]) = ((number%2)<1);
				number = number/2;
		}	
}

int main(void)
{
	uint32_t i =0;
	uint32_t lasti = 0;
	SYS_Init();
	OpenKeyPad();
	GPIO_SetMode(PB, BIT11, GPIO_MODE_OUTPUT);
 	while(1) 
  {
		i=ScanKey();
		if(i!=0){
				lasti = i;
				continue;
		}	
		if(lasti>0&&lasti<=6){
				showbinary(lasti);
				for(int r=0;r<lasti;r++){
					PB11=0;
					CLK_SysTickDelay(100000);	 // Delay 
					PB11=1;
					CLK_SysTickDelay(100000);	 // Delay 					
				}
				lasti =0 ;
		}
	}
}


