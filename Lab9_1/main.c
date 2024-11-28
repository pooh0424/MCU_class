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
#include "Seven_segment.h"
#include "Draw2D.h"

volatile _Bool run =0;
volatile int32_t direction =0;
volatile uint32_t* leds[3] = { &PA12, &PA13, &PA14};
volatile uint32_t runvalue;
volatile uint8_t flag =0;
void ADC_IRQHandler(void)
{
    uint32_t u32Flag;
    // Get ADC conversion finish interrupt flag
    u32Flag = ADC_GET_INT_FLAG(ADC, ADC_ADF_INT);

    if(u32Flag & ADC_ADF_INT){
				flag=1;
		}
    ADC_CLR_INT_FLAG(ADC, u32Flag);
}

void GPAB_IRQHandler(void)
{
			if (PA->ISRC & BIT0) {        // check if PB12 interrupt occurred
					PA->ISRC |= BIT0;			// clear PB12 interrupt status

			} 
			else if (PA->ISRC & BIT1) { // check if Pb13 interrupt occurred
					PA->ISRC |= BIT1;         // clear PB13 interrupt status
					run = !run;
					direction = 1;
			} 
			else if (PA->ISRC & BIT2) { // check if PB14 interrupt occurred
					PA->ISRC |= BIT2;         // clear PB14 interrupt status  		

			} 
			else {                      // else it is unexpected interrupts
					PA->ISRC = PA->ISRC;	      // clear all GPB pins
			}
}

void Init_KEYandADC(void)
{
		GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PA, BIT4, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PA, BIT5, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, (BIT0 | BIT1| BIT2), GPIO_MODE_QUASI);
		PA3=1; PA4=0; PA5=1;
    GPIO_EnableInt(PA, 0, GPIO_INT_FALLING);
    GPIO_EnableInt(PA, 1, GPIO_INT_FALLING);
    GPIO_EnableInt(PA, 2, GPIO_INT_FALLING);		
    NVIC_EnableIRQ(GPAB_IRQn);   
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_64);
    GPIO_ENABLE_DEBOUNCE(PA, (BIT0 | BIT1| BIT2));
    ADC_Open(ADC, ADC_INPUT_MODE, ADC_OPERATION_MODE, ADC_CHANNEL_MASK);
    ADC_POWER_ON(ADC);
    ADC_EnableInt(ADC, ADC_ADF_INT);
    NVIC_EnableIRQ(ADC_IRQn);
		ADC_START_CONV(ADC);
}


int main(void)
{
	
	uint32_t count = 0;
	uint16_t fgColor, bgColor;
	int32_t posx=52,posy=30;
	SYS_Init();
	init_LCD();
	clear_LCD();
	Init_KEYandADC();
	OpenSevenSegment();
	bgColor = BG_COLOR;
	fgColor = FG_COLOR;
	fill_Rectangle(posx, posy, posx+15, posy+7,fgColor, bgColor); 
	uint32_t runtime[3]={200,100,50};
	while(1) {
			if(flag==1){
				int value = ADC_GET_CONVERSION_DATA(ADC, 7);
				if(value<1365){
						runvalue = 1;
				}
				else if(value<3500){
						runvalue = 2;
				}
				else{
						runvalue = 3;
				}
			}
		if(run){
			if(count%(runtime[runvalue-1])==0){
				fgColor = BG_COLOR;
				fill_Rectangle(posx, posy, posx+15, posy+7,fgColor, bgColor); 	
				if(posx+15+direction>=128){
						direction = -1;
				}
				if(posx+direction<0){
						direction = 1;
				}
				posx = posx + direction;
				fgColor = FG_COLOR;
				fill_Rectangle(posx, posy, posx+15, posy+7,fgColor, bgColor); 
			}
			if(count==399){
				count=0;
			}
			else{
				count++;
			}
		}
		for(int i = 0;i<3;i++){
			*leds[i]= 1;
		}
		ShowSevenSegment(0,runvalue);
		*leds[runvalue-1]=0;
	  CLK_SysTickDelay(200);		
	}
}

