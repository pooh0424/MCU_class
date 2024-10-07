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
void Display_7seg(uint32_t leftvalue,uint32_t value)
{
  uint8_t digit;
			
	CloseSevenSegment();
	ShowSevenSegment(3,leftvalue);
	CLK_SysTickDelay(5000);
	
	digit = value/10;
	CloseSevenSegment();
	ShowSevenSegment(1,digit);
	CLK_SysTickDelay(5000);

	digit = value%10;
	CloseSevenSegment();
	ShowSevenSegment(0,digit);
	CLK_SysTickDelay(5000);
}
int main(void)
{
	uint32_t count =0;
	uint32_t i =0;
	uint32_t last_i = 0;
	int32_t number=-1;
	SYS_Init();
	OpenKeyPad();
	OpenSevenSegment();

 	while(1) 
  {
		count++;
		if(number>=0){
			Display_7seg(number%3,number);
			switch (number%3){
				case 0:
					PC12=0; PC13=0; PC14=0; PC15=0;
					CLK_SysTickDelay(100000);	
					PC12=1; PC13=1; PC14=1; PC15=1;
					CLK_SysTickDelay(100000);	
					break;
				case 1:
					PC12=0; PC13=1; PC14=1; PC15=1;
					PB11=0;
					CLK_SysTickDelay(100000);	
					PC12=1; PC13=1; PC14=1; PC15=1;
					PB11=1;
					CLK_SysTickDelay(100000);	
					break;
				case 2:
					PC12=0; PC13=1; PC14=1; PC15=1;
					for(int r=0;r<52;r++){
							PB11=0;
							CLK_SysTickDelay(956);	
							PB11=1;
							CLK_SysTickDelay(956);	
					}
					PC12=1; PC13=1; PC14=1; PC15=0;
					for(int r=0;r<98;r++){
							PB11=0;
							CLK_SysTickDelay(506);	
							PB11=1;			
							CLK_SysTickDelay(506);							
					}	
					break;
			}
		}
		i=ScanKey();
		if(i!=0){
				last_i=i;
				continue;
		}
		if(last_i == 9){
			srand(count);
			number=rand()%100;
			last_i =0;
		}
		else if(last_i==8){
			number=-1;
			CloseSevenSegment();
			PC12=1; PC13=1; PC14=1; PC15=1;
			last_i =0;
		}	
		CLK_SysTickDelay(5000);
	}
}


