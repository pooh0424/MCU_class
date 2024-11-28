//
// LCD_Graph_Pingpong: draw a circle bouncing around between two bars on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Draw2D.h"
#include "Scankey.h"

#define PIXEL_ON  1
#define PIXEL_OFF 0


int32_t main (void)
{
	int dirX=0, dirY=0;
	uint16_t r = 3;
	uint16_t x=64, y=60;
	uint16_t squx, squy;
	uint16_t fgColor, bgColor;
	uint32_t i =0;
	uint32_t last_i = 0;	
	uint32_t count = 0;
	uint32_t seed;	
	SYS_Init();
	init_LCD();  
	clear_LCD();	
	OpenKeyPad(); 
	srand(seed);
	squx = rand()%123;
	squy = rand()%28;
	
	bgColor = BG_COLOR;
	fgColor = FG_COLOR;
	draw_Circle(x, y, r, fgColor, bgColor);
	fill_Rectangle(squx, squy, squx+5, squy+5,fgColor, bgColor); 
	while(1) {
		if(!(dirX==0&&dirY==0)){
				if(count==0){
						count++;
						if ((x+dirX) < r){ 
								dirX=1;
								PB11=0;
								CLK_SysTickDelay(10000);			
								PB11=1;	
						}
						if ((x+dirX) > LCD_Xmax-r){
								dirX=-1;
								PB11=0;
								CLK_SysTickDelay(10000);			
								PB11=1;	
						}
						if ((y+dirY) < r){
								dirY=1;
								PB11=0;
								CLK_SysTickDelay(10000);			
								PB11=1;	
						}
						if ((y+dirY) > LCD_Ymax-r){
								dirY=-1;
								PB11=0;
								CLK_SysTickDelay(10000);			
								PB11=1;	
						}
						if(x >= (squx-r-1)&&(x <= squx+5+r+1)&&(y >= squy-r-1)&&(y <= squy+5+r+1)){
								fgColor = BG_COLOR;
								fill_Rectangle(squx, squy, squx+5, squy+5,fgColor, bgColor); 
								fgColor = BG_COLOR;
								draw_Circle(x, y, r, fgColor, bgColor);
								x=64;
								y=60;
								dirX = 0;
								dirY = 0;
								count = 0;									
						}
						fgColor = BG_COLOR;
						draw_Circle(x, y, r, fgColor, bgColor); // erase a circle
						x = x + dirX; // change x of circle center
						y = y + dirY; // change y of circle center			
					  fgColor = FG_COLOR;
						draw_Circle(x, y, r, fgColor, bgColor); // draw a circle
				}
				else if(count ==500){
						count=0;
				}
				else{
						count++;
				}
		}
		CLK_SysTickDelay(200);
		i=ScanKey();
		if(i!=0){
			last_i=i;
			continue;
		}
		if(last_i == 1 && (dirX==0&&dirY==0)){
				dirX = -1;
				dirY = -1;
				last_i=0;
		}
		else if(last_i == 3&& (dirX==0&&dirY==0)){
				dirX = 1;
				dirY = -1;
				last_i=0;
		}	
		else if(last_i == 4&& (dirX==0&&dirY==0)){
				dirX = -1;
				dirY = 0;
				last_i=0;
		}	
		else if(last_i == 6&& (dirX==0&&dirY==0)){
				dirX = 1;
				dirY = 0;
				last_i=0;
		}	
		else if(last_i == 7&& (dirX==0&&dirY==0)){
				dirX = -1;
				dirY = 1;
				last_i=0;
		}	
		else if(last_i == 9&& (dirX==0&&dirY==0)){
				dirX = 1;
				dirY = 1;
				last_i=0;
		}	
		else if(last_i==5){
				dirX = 0;
				dirY = 0;
				count = 0;
				last_i=0;
		}
		else if(last_i==8){
			fgColor = BG_COLOR;
			fill_Rectangle(squx, squy, squx+5, squy+5,fgColor, bgColor); // erase a circle
			srand(seed);
			squx = rand()%123;
			squy = rand()%28;
			fgColor = FG_COLOR;
			fill_Rectangle(squx, squy, squx+5, squy+5,fgColor, bgColor);
			last_i=0;
		}
		else{
			last_i=0;
		}
		seed++;
	}

}