//
// LCD_Bmp : display Logo Bitmap on LCD
//
// Bitmap Creation:
// Step 1. use Paint to draw 128x64 bitmap 
// Step 2. use bmp2asm to convert bitmap into hex
// Step 3. cut hex code paste into char array Bitmap[128*8]
// Step 4. build & download code to learning board, then bitmap will be displayed on LCD
//
#include <stdio.h>
#include <stdlib.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"


volatile uint32_t* leds[4] = {&PC12, &PC13, &PC14, &PC15};
int move[4] = {0,0,0,0};
int scount;
int number[4];
int xpos[4] = {0,0,0,0};
void EINT1_IRQHandler(void)
{
    GPIO_CLR_INT_FLAG(PB, BIT15);  // Clear GPIO interrupt flag

		int id[4]={0,1,2,3};
		for(int i=0;i<4;i++){
			for(int j=0;j<4-i-1;j++){
					if(number[id[j]]>number[id[j+1]]){
							int buffer=id[j];
							id[j] = id[j+1];
							id[j+1] = buffer;
					}
			}
		}
		for(int i=0;i<4;i++){
			move[id[i]]=(i+1)*2;
		}
}


int contains(int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return 1;
        }
    }
    return 0;
}

void GPAB_IRQHandler(void)
{
		if(move[0]==0&&move[1]==0&&move[2]==0&&move[3]==0){
			if (PA->ISRC & BIT0) {        // check if PB12 interrupt occurred
					PA->ISRC |= BIT0;			// clear PB12 interrupt status
					srand(scount);
					for(int i=0;i<4;i++){
						int newNumber;
						do {
								newNumber = rand() % 9+1; 
						} while (contains(number, i, newNumber)); 
						number[i] = newNumber;
						xpos[i] =0;
						*leds[i]=1;
					}
					clear_LCD();
					for(int i=0;i<4;i++){
							printC(xpos[i],i*16,number[i]+48);
					}
			} else if (PA->ISRC & BIT1) { // check if Pb13 interrupt occurred
					PA->ISRC |= BIT1;         // clear PB13 interrupt status
					srand(scount);
					for(int i=0;i<4;i++){
						int newNumber;
						do {
								newNumber = rand() % 9+1; 
						} while (contains(number, i, newNumber)); 
						number[i] = newNumber;
						xpos[i] =0;
						*leds[i]=1;
					}			
					clear_LCD();
					for(int i=0;i<4;i++){
							printC(xpos[i],i*16,number[i]+48);
					}
			} else if (PA->ISRC & BIT2) { // check if PB14 interrupt occurred
					PA->ISRC |= BIT2;         // clear PB14 interrupt status  		
					srand(scount);
					for(int i=0;i<4;i++){
						int newNumber;
						do {
								newNumber = rand() % 9+1; 
						} while (contains(number, i, newNumber)); 
						number[i] = newNumber;
						xpos[i] =0;
						*leds[i]=1;
					}		
					clear_LCD();
					for(int i=0;i<4;i++){
							printC(xpos[i],i*16,number[i]+48);
					}
			} else {                      // else it is unexpected interrupts
					PA->ISRC = PA->ISRC;	      // clear all GPB pins
			}
		}
		else {                      // else it is unexpected interrupts
        PA->ISRC = PA->ISRC;	      // clear all GPB pins
    }
}

void Init_KEY(void)
{
		GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PA, BIT4, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PA, BIT5, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, (BIT0 | BIT1| BIT2), GPIO_MODE_QUASI);
		PA3=0; PA4=0; PA5=0;
    GPIO_EnableInt(PA, 0, GPIO_INT_FALLING);
    GPIO_EnableInt(PA, 1, GPIO_INT_FALLING);
    GPIO_EnableInt(PA, 2, GPIO_INT_FALLING);		
    NVIC_EnableIRQ(GPAB_IRQn);   
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_64);
    GPIO_ENABLE_DEBOUNCE(PA, (BIT0 | BIT1| BIT2));
    GPIO_SetMode(PB, BIT15, GPIO_MODE_INPUT);
    GPIO_EnableEINT1(PB, 15, GPIO_INT_RISING); // RISING, FALLING, BOTH_EDGE, HIGH, LOW
    NVIC_EnableIRQ(EINT1_IRQn);

    // Enable interrupt de-bounce function and select de-bounce sampling cycle time
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_64);
    GPIO_ENABLE_DEBOUNCE(PB, BIT15);	
}


int main(void)
{
	uint32_t count = 0;
	SYS_Init();
	init_LCD();
	clear_LCD();
	Init_KEY();
	srand(scount);
	for(int i=0;i<4;i++){
		int newNumber;
		do {
				newNumber = rand() % 9+1; 
		} while (contains(number, i, newNumber));
		number[i] = newNumber;
		xpos[i] =0;
		*leds[i]=1;
	}
	for(int i=0;i<4;i++){
			printC(xpos[i],i*16,number[i]+48);
	}
	while(1) {
		if(!(move[0]==0&&move[1]==0&&move[2]==0&&move[3]==0)){
			if(count==200){
				count=0;
				clear_LCD();
				for(int i=0;i<4;i++){
						if(xpos[i]+move[i]>120){
								move[i]=0;
								*leds[i]=0;
						}
						else{
								xpos[i]+=move[i];
						}
						printC(xpos[i],i*16,number[i]+48);
				}
			}
			else{
				count++;
			}
		}
	  CLK_SysTickDelay(200);		
		scount++;
	}
}

