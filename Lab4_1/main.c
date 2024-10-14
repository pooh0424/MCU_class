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
volatile uint32_t* leds[3] = {&PA14, &PA13, &PA12};


void Init_GPIO(void)
{
	  GPIO_SetMode(PA, BIT12, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PA, BIT13, GPIO_MODE_OUTPUT);
	  GPIO_SetMode(PA, BIT14, GPIO_MODE_OUTPUT);
	  PA12=1; PA13=1; PA14=1;
}
void Display_7seg(uint32_t value[3],uint32_t delay)
{
	while(delay>15000){
		CloseSevenSegment();
		ShowSevenSegment(2,value[0]);
		CLK_SysTickDelay(5000);
		
		CloseSevenSegment();
		ShowSevenSegment(1,value[1]);
		CLK_SysTickDelay(5000);

		CloseSevenSegment();
		ShowSevenSegment(0,value[2]);
		CLK_SysTickDelay(5000);
		delay-=15000;
	}
	CloseSevenSegment();
	ShowSevenSegment(2,value[0]);
	CLK_SysTickDelay(delay/3);
		
	CloseSevenSegment();
	ShowSevenSegment(1,value[1]);
	CLK_SysTickDelay(delay/3);

	CloseSevenSegment();
	ShowSevenSegment(0,value[2]);
	CLK_SysTickDelay(delay/3);
	
}
int main(void)
{
	uint32_t count =0;
	uint32_t count2 =0;
	uint32_t state =0;
	uint32_t i =0;
	uint32_t last_i = 0;
	uint32_t number[3]={0,0,0};
	SYS_Init();
	OpenKeyPad();
	OpenSevenSegment();

 	while(1) 
  {
		if(state!=0){
				if(count%300==0){
						  PA12=1; PA13=1; PA14=1;
							leds[count/300]=0;
							if(state==1){
									uint32_t buffer=number[0];
									number[0]=number[1];
									number[1]=number[2];
									number[2]=buffer;
							}
							else{
									uint32_t buffer=number[2];
									number[2]=number[1];
									number[1]=number[0];
									number[0]=buffer;							
							}
				}
				count2++;
				if(count2%900==0)count2=0;
		}
		Display_7seg(number,1000);
		i=ScanKey();
		if(i!=0){
				last_i=i;
				continue;
		}
		if(last_i>=1&&last_i<=6){
				number[0]=number[1];
				number[1]=number[2];
				number[2]=last_i;
				state=0;
				last_i = 0;
		}
		else if(last_i==8){
				for(int i=0;i<3;i++){
						number[i]=0;
				}
				CloseSevenSegment();
				PA12=1; PA13=1; PA14=1;	
				state=0;
				last_i = 0;
		}	
		else if(last_i==7){
				count2=0;
				if(number[0]!=0)state=1;
				last_i = 0;
		}	
		else if(last_i==9){
				count2=0;
				if(number[0]!=0)state=2;
				last_i = 0;
		}	
		
	}
}


