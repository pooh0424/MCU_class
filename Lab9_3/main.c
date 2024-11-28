#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Draw2D.h"
#include "Scankey.h"
volatile uint8_t KEY1_Flag = 0;

volatile uint8_t u8ADF;
volatile uint16_t X, Y;
volatile uint8_t  B;
void ADC_IRQHandler(void)
{
    uint32_t u32Flag;

    // Get ADC conversion finish interrupt flag
    u32Flag = ADC_GET_INT_FLAG(ADC, ADC_ADF_INT);

    if(u32Flag & ADC_ADF_INT) {
        X = ADC_GET_CONVERSION_DATA(ADC, 0);
    }
    ADC_CLR_INT_FLAG(ADC, u32Flag);
}

void Init_ADC(void)
{
    ADC_Open(ADC, ADC_INPUT_MODE, ADC_OPERATION_MODE, ADC_CHANNEL_MASK );
    ADC_POWER_ON(ADC);
    ADC_EnableInt(ADC, ADC_ADF_INT);
    NVIC_EnableIRQ(ADC_IRQn);
    ADC_START_CONV(ADC);	
}

int32_t main (void)
{
    SYS_Init();
    Init_ADC(); 
    init_LCD();
    clear_LCD();
    GPIO_SetMode(PB, BIT11, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT0, GPIO_MODE_QUASI);
		OpenKeyPad();
		int k = 0;
		int kp = 0;
    int count = 0;
    uint16_t fgColor = FG_COLOR;
    uint16_t bgColor = BG_COLOR;
    int PLAT_X = 64;
    int PLAT_Y = 60;
    fill_Rectangle(PLAT_X - 8, PLAT_Y - 4, PLAT_X + 8, PLAT_Y + 4,fgColor ,bgColor);
    srand(X);
    int rand_X = rand() % 112 + 8;
    int rand_Y = rand() % 24 + 4;
    fill_Rectangle(rand_X - 8,rand_Y - 4, rand_X + 8, rand_Y + 4,fgColor ,bgColor);
    
		while(KEY1_Flag == 0){
			count++;
			k = ScanKey();
			if (k != 0){
				kp = k;
				continue;
			}
			if(kp == 1 || kp == 4 || kp == 7){
				KEY1_Flag = 1;
				kp = 0;
			}
		}
    srand(count);
    int dir_X = rand() % 2;
    if (dir_X == 0) dir_X = -1;
    int dir_Y = rand() % 2;
    if (dir_Y == 0) dir_Y = -1;
    int ball_X = 64;
    int ball_Y = 32;
    fill_Rectangle(ball_X - 4,ball_Y - 4, ball_X + 4, ball_Y + 4,fgColor ,bgColor);
    while(1){
        if(ball_Y == 60){
            clear_LCD();
            printS(28,32,"Game Over");
            break;
        }
        if(ball_X - 4 == 0 || ball_X + 4 == 128) dir_X = -dir_X;
        if(ball_Y - 4 == 0) dir_Y = -dir_Y;
        
        if(ball_X  - 4 <= rand_X + 9 && ball_X + 4 >= rand_X - 9 && ball_Y - 4 <= rand_Y + 5 && ball_Y + 4 >= rand_Y - 5){
            PB11 = 0;
            if(ball_X - 4 == rand_X + 9 || ball_X + 4 == rand_X - 9) dir_X = -dir_X;
            else dir_Y = -dir_Y;
            CLK_SysTickDelay(10000);
            PB11 = 1;
        }
        if(ball_X - 4 <= PLAT_X + 9 && ball_X + 4 >= PLAT_X - 9 && ball_Y - 4 <= PLAT_Y + 5 && ball_Y + 4 >= PLAT_Y - 5){
            PB11 = 0;
            if(ball_X - 4 == PLAT_X + 9 || ball_X + 4 == PLAT_X - 9) dir_X = -dir_X;
            else dir_Y = -dir_Y;
            CLK_SysTickDelay(10000);
            PB11 = 1;
        }
        fill_Rectangle(PLAT_X - 8, PLAT_Y - 4, PLAT_X + 8, PLAT_Y + 4,bgColor ,bgColor);
        if(X <= 1848 && PLAT_X - 8 >= 0) PLAT_X--;
        if(X >= 2248 && PLAT_X + 8 <= 127) PLAT_X++;
        fill_Rectangle(PLAT_X - 8, PLAT_Y - 4, PLAT_X + 8, PLAT_Y + 4,fgColor ,bgColor);

        fill_Rectangle(ball_X - 4,ball_Y - 4, ball_X + 4, ball_Y + 4,bgColor ,bgColor);
        ball_X += dir_X;
        ball_Y += dir_Y;
        fill_Rectangle(ball_X - 4,ball_Y - 4, ball_X + 4, ball_Y + 4,fgColor ,bgColor);
        
        CLK_SysTickDelay(20000);
    }
}

