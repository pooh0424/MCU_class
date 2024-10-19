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
#include "LCD.h"

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
	int32_t src=0;
	int32_t enternumber=0;
	int32_t linenew=0;
	char Text[16] = "               ";
	volatile uint32_t* leds[4] = {&PC12, &PC13, &PC14, &PC15};
	SYS_Init();
	OpenKeyPad();
	OpenSevenSegment();
	init_LCD();
	clear_LCD();

 	while(1) 
  {
		Display_7seg(src);
		count++;
		i=ScanKey();
		if(i!=0){
				last_i=i;
				continue;
		}
		if(last_i == 7){
			srand(count);
			src =((rand()%6)+1)*1000+((rand()%6)+1)*100+((rand()%6)+1)*10+((rand()%6)+1);
			last_i =0;
		}
		else if(last_i>0 && last_i<7 && enternumber<1000&&linenew <4){
			enternumber = enternumber*10+last_i;
			sprintf(Text,"%d",enternumber); 	  
			print_Line(linenew, Text);         
			last_i =0;
		}	
		else if(last_i==8){
			clear_LCD();
			linenew = 0;
			enternumber = 0;
			last_i =0;
		}
		else if(last_i==9&&linenew<4){
			if(enternumber == src){
				sprintf(Text+8,"PASS"); 			  
				print_Line(linenew, Text);  
				for (int i=0;i<4;i++){
						*leds[i] =0;
						Display_7seg(src);		
						*leds[i] =1;	
						Display_7seg(src);					
				}
				for (int i=3;i>=0;i--){
						*leds[i] =0;
						Display_7seg(src);		
						*leds[i] =1;	
						Display_7seg(src);					
				}
				for (int i=0;i<4;i++){
						*leds[i] =0;
						Display_7seg(src);		
						*leds[i] =1;	
						Display_7seg(src);					
				}
				for (int i=3;i>=0;i--){
						*leds[i] =0;
						Display_7seg(src);		
						*leds[i] =1;	
						Display_7seg(src);					
				}
			}	
			else if(enternumber == 0){
				sprintf(Text+8,"NULL"); 	  
				print_Line(linenew, Text);

			}
			else{
				sprintf(Text+8,"ERROR"); 	  
				print_Line(linenew, Text);
				PB11=0;
				Display_7seg(src);		
				PB11=1;				
			}
			last_i =0;
			linenew++;
			enternumber =0;
		}
	}
}


