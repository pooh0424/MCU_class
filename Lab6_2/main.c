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
#include "LCD.h"

void showlcd(int32_t number[],uint32_t sum,uint32_t mospos,uint32_t state){
	char Text[100];
	clear_LCD();
	sprintf(Text,"  sum=%d     ",sum); 	  
	print_Line(0, Text);
	for(int i=0;i<3;i++){
			if(i==mospos-state){
				sprintf(Text,">  %-2d        ",number[i+state]);
				print_Line(i+1, Text);
			}
			else{
				sprintf(Text,"   %-2d        ",number[i+state]);
				print_Line(i+1, Text);
			}
	}
}

int main(void)
{
	
	uint32_t count =0;
	uint32_t i =0;
	uint32_t last_i = 0;
	uint32_t state=0;
	uint32_t mospos=0;
	uint32_t sum=0;
	int32_t number[4]={0,0,0,0};
	int32_t stack[4]={0,0,0,0};
	int32_t stacktop=-1;
	volatile uint32_t* leds[4] = {&PC12, &PC13, &PC14, &PC15};
	SYS_Init();
	OpenKeyPad();
	init_LCD();
	clear_LCD();
	showlcd(number,sum,mospos,state);
 	while(1) 
  {
		count++;
		i=ScanKey();
		if(i!=0){
			last_i=i;
			continue;
		}
		if(last_i == 7&& stacktop==-1){
			srand(count);
			for(int i=0;i<4;i++){
				number[i] = rand()%90+10;
			}
			showlcd(number,sum,mospos,state);
			last_i =0;
		}
		else if(last_i == 4){
			if(mospos==0){
			}
			else{
				mospos--;
			}
			if(mospos== 0){
				state=0;
			}  
			showlcd(number,sum,mospos,state);
			last_i =0;
		}	
		else if(last_i == 6){
			if(mospos==3){
			}
			else{
				mospos++;
			}
			if(mospos== 3){
				state=1;
			}
			showlcd(number,sum,mospos,state);			
			last_i =0;
		}	
		else if(last_i==9){
			stacktop =-1;
			sum=0;
			last_i =0;
			for(int i=0;i<4;i++){
				*leds[i]=1;
			}
			showlcd(number,sum,mospos,state);
		}
		else if(last_i==8&&stacktop!=-1){
			sum-=stack[stacktop];
			stacktop --;
			last_i =0;
			for(int i=0;i<4;i++){
				if(i<stacktop+1){
						*leds[i]=0;
				}
				else{
						*leds[i]=1;
				}
			}
			showlcd(number,sum,mospos,state);
		}	
		else if(last_i==5&&stacktop!=3){
			sum+=number[mospos];
			stacktop++;
			stack[stacktop] = number[mospos];
			last_i =0;
			for(int i=0;i<4;i++){
				if(i<stacktop+1){
						*leds[i]=0;
				}
				else{
						*leds[i]=1;
				}
			}
			showlcd(number,sum,mospos,state);
			PB11=0;
			CLK_SysTickDelay(10000);			
			PB11=1;			
		}	
		CLK_SysTickDelay(500);			
	}
}


