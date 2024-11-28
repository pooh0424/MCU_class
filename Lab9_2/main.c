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

volatile _Bool run =1;
volatile int dirX=0, dirY=0;
volatile uint16_t x=60, y=28;
volatile int downx;
volatile int seed;
volatile uint16_t squx, squy;
volatile uint8_t flag =0;
void ADC_IRQHandler(void)
{
    uint32_t u32Flag;
    // Get ADC conversion finish interrupt flag
    u32Flag = ADC_GET_INT_FLAG(ADC, ADC_ADF_INT);

    if((u32Flag & ADC_ADF_INT) && run){
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
			} 
			else if (PA->ISRC & BIT2) { // check if PB14 interrupt occurred
					PA->ISRC |= BIT2;         // clear PB14 interrupt status  		
					srand(seed);
					x=60;
					y=28;
					dirX = (rand()%2)*2-1;
					dirY = (rand()%2)*2-1;
					squx = rand()%112;
					squy = rand()%15;
					clear_LCD();
					uint16_t fgColor, bgColor;
					bgColor = BG_COLOR;
					fgColor = FG_COLOR;
					fill_Rectangle(squx, squy, squx+15, squy+7,fgColor, bgColor); 
					fill_Rectangle(x, y, x+7, y+7,fgColor, bgColor); 
					run = TRUE;

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
		PA3=0; PA4=0; PA5=0;
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
	SYS_Init();
	init_LCD();
	clear_LCD();
	Init_KEYandADC();
	srand(seed);
	squx = rand()%112;
	squy = rand()%15;
	bgColor = BG_COLOR;
	fgColor = FG_COLOR;
	fill_Rectangle(squx, squy, squx+15, squy+7,fgColor, bgColor); 
	fill_Rectangle(x, y, x+7, y+7,fgColor, bgColor); 
	while(1) {
		if(run){

			if(count==20){		
					if(flag ==1){
						int value = ADC_GET_CONVERSION_DATA(ADC, 0);
						uint16_t fgColor, bgColor;
						bgColor = BG_COLOR;
						fgColor = BG_COLOR;
						fill_Rectangle(downx, 50, downx+15, 57,fgColor, bgColor); 
						if(value<100){
								 downx= 0;
						}
						else if(value>3996){
								 downx= 112;
						}
						else{
								 downx = INT16_C((value-100)*(112/3896.0));
						}
						fgColor = FG_COLOR;
						fill_Rectangle(downx, 50, downx+15, 57,fgColor, bgColor); 			
					}				
					if ((x+dirX) <= squx+15 &&(x+dirX+7) >= squx && (y+dirY) <= squy+7&&(y+dirY+7) >= squy){
							if((x+dirX) == squx+15){
									dirX=1;
							}
							if ((x+dirX+7) == squx){
									dirX=-1;	
							}
							if ((y+dirY) == squy+7){
									dirY=1;
							}
							if ((y+dirY+7) == squy){
									dirY=-1;
							}
							PB11= 0;
							CLK_SysTickDelay(400);
							PB11 = 1;
					}
					if ((x+dirX) <= downx+15 &&(x+dirX+7) >= downx && (y+dirY) <= 57&&(y+dirY+7) >= 50){
							if((x+dirX) == downx+15){
									dirX=1;
							}
							if ((x+dirX+7) == downx){
									dirX=-1;	
							}
							if ((y+dirY) == 57){
									dirY=1;
							}
							if ((y+dirY+7) == 50){
									dirY=-1;
							}
							PB11= 0;
							CLK_SysTickDelay(400);
							PB11 = 1;
					}
					if ((x+dirX) < 0){ 
							dirX=1;
					}
					if ((x+dirX+7) >= LCD_Xmax){
							dirX=-1;	
					}
					if ((y+dirY) < 0){
							dirY=1;
					}
					if ((y+dirY+7) >= LCD_Ymax){
							run = 0;
							clear_LCD();
							printS(30,28,"Game over");
					}
					if(run){
							fgColor = BG_COLOR;
							fill_Rectangle(x, y, x+7, y+7,fgColor, bgColor); 
							x = x+dirX;
							y = y+dirY;
							fgColor = FG_COLOR;
							fill_Rectangle(squx, squy, squx+15, squy+7,fgColor, bgColor);
							fill_Rectangle(downx, 50, downx+15, 57,fgColor, bgColor); 			
							fill_Rectangle(x, y, x+7, y+7,fgColor, bgColor); 
					}
					count = 0;
			}
			else{
				count++;
			}
		}
	  CLK_SysTickDelay(200);	
		seed++;
	}
}

