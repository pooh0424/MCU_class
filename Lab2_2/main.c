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
#include "Seven_Segment.h"
volatile uint32_t* leds[4] = {&PC12, &PC13, &PC14, &PC15};

void Init_GPIO(void)
{
	  GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT14, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PC, BIT15, GPIO_MODE_OUTPUT);
	  PC12=1; PC13=1; PC14=1; PC15=1;
}
void Display_7seg(uint32_t value[3])
{
  uint8_t digit;
			
	digit = value[0];
	CloseSevenSegment();
	ShowSevenSegment(2,digit);
	CLK_SysTickDelay(5000);

	digit = value[1];
	CloseSevenSegment();
	ShowSevenSegment(1,digit);
	CLK_SysTickDelay(5000);

	digit = value[2];
	CloseSevenSegment();
	ShowSevenSegment(0,digit);
	CLK_SysTickDelay(5000);
}
int main(void)
{
	uint32_t i =0;
	uint32_t last_i = 0;
	uint32_t number[3]={0,0,0};
	SYS_Init();
	OpenKeyPad();
	OpenSevenSegment();

 	while(1) 
  {
		i=ScanKey();
		Display_7seg(number);
		if(i!=0){
				last_i=i;
				continue;
		}
		if(last_i == 8){
				number[0]=0;
				number[1]=0;
				number[2]=0;
				last_i = 0;
		}
		else if(last_i==7){
				number[2]=number[1];
				number[1]=number[0];
				number[0]=0;
				last_i = 0;
		}	
		else if(last_i>0&&last_i<7){
				number[0]=number[1];
				number[1]=number[2];
				number[2]=last_i;
				last_i = 0;
		}	
		CLK_SysTickDelay(5000);
		
	}
}


