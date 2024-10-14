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

void Display_7seg(int32_t value)
{
		uint8_t digit;
		CloseSevenSegment();
		ShowSevenSegment(3,value/1000);
		CLK_SysTickDelay(5000);
	
		digit = (value%1000)/100;
		CloseSevenSegment();
		ShowSevenSegment(2,digit);
		CLK_SysTickDelay(5000);
		
		digit = (value%100)/10;
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
	int32_t state = -1;
	int32_t numberA=0;
	int32_t numberB=0;
	int32_t resultnumber=0;
	SYS_Init();
	OpenKeyPad();
	OpenSevenSegment();

 	while(1) 
  {
		if(state==0){
			Display_7seg(numberA*100+numberB);
		}
		else if(state==1){
			Display_7seg(resultnumber);
		}
		count++;
		i=ScanKey();
		if(i!=0){
				last_i=i;
				continue;
		}
		if(last_i == 9){
			srand(count);
			numberA=((rand()%9)+1)*10+(rand()%9)+1;
			numberB=((rand()%9)+1)*10+(rand()%9)+1;
			while(numberA==numberB){
					numberA=((rand()%9)+1)*10+(rand()%9)+1;
					numberB=((rand()%9)+1)*10+(rand()%9)+1;
			}
			state =0;
			last_i =0;
		}
		else if(last_i==1&&state!=-1){
			resultnumber=numberA+numberB;
			state =1;
			last_i =0;
		}	
		else if(last_i==2&&state!=-1){
			resultnumber=numberA-numberB;
			if(resultnumber<0){
					resultnumber=-resultnumber;
					PB11=0;
					CLK_SysTickDelay(100000);
					PB11=1;
				
			}
			state =1;
			last_i =0;
		}	
		else if(last_i==8){
		  CloseSevenSegment();
			state =-1;
			last_i =0;
		}	
	}
}


