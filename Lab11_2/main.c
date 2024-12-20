//
// PWM_Music : PWM0 Ch0 (PA12) output music "Fur Elise"
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN  (LQPF-100)
// Buzzer: used as speaker

// PA12/PWM0 connected to PB11
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Note_Freq.h"
#include "Seven_Segment.h"

#define  P125ms 125000
#define  P250ms 250000
#define  P500ms 500000
#define  P1S   1000000
volatile uint16_t song=0;
volatile uint8_t i;
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
					song = 2;
					i= 0;
			} 
			else if (PA->ISRC & BIT1) { // check if Pb13 interrupt occurred
					PA->ISRC |= BIT1;         // clear PB13 interrupt status
					song = 1;
					i= 0;
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
int32_t main (void)
{

  
  uint16_t music2[72] = {
	E6 ,D6u,E6 ,D6u,E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0 ,
	C5 ,E5 ,A5 ,B5 ,B5 , 0 ,C5 ,A5 ,B5 ,C6 ,C6 , 0 ,
	E6 ,D6u,E6 ,D6u,E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0 ,
	C5 ,E5 ,A5 ,B5 ,B5 , 0 ,E5 ,C6 ,B5 ,A5 ,A5 , 0 ,
	B5 ,C6 ,D6 ,E6 ,E6 , 0 ,G5 ,F6 ,E6 ,D6 ,D6 , 0 ,
	F5 ,E6 ,D6 ,C6 ,C6 , 0 ,E5 ,D6 ,C6 ,B5 ,B5 , 0 };
	
  uint32_t pitch2[72] = {
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms
	};
	uint16_t music1[48] = {
			C4, C4, G4, G4, A4, A4, G4,0 , // Twinkle, Twinkle, Little Star
			F4, F4, E4, E4, D4, D4, C4,0 , // How I wonder what you are
			G4, G4, F4, F4, E4, E4, D4,0 , // Up above the world so high
			G4, G4, F4, F4, E4, E4, D4,0 , // Like a diamond in the sky
			C4, C4, G4, G4, A4, A4, G4,0 , // Twinkle, Twinkle, Little Star
			F4, F4, E4, E4, D4, D4, C4,0   // How I wonder what you are
	};

	uint32_t pitch1[48] = {
			P500ms, P500ms, P500ms, P500ms, P500ms, P500ms, P1S,P1S,  // ???
			P500ms, P500ms, P500ms, P500ms, P500ms, P500ms, P1S,P1S,  // ???
			P500ms, P500ms, P500ms, P500ms, P500ms, P500ms, P1S,P1S,  // ???
			P500ms, P500ms, P500ms, P500ms, P500ms, P500ms, P1S,P1S,  // ???
			P500ms, P500ms, P500ms, P500ms, P500ms, P500ms, P1S,P1S,  // ???
			P500ms, P500ms, P500ms, P500ms, P500ms, P500ms, P1S,P1S   // ???
	};
		int volume = 50;
    SYS_Init();
		OpenSevenSegment();

    PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
    PWM_Start(PWM0, PWM_CH_0_MASK);	
		Init_KEYandADC();
		while(1){
			if(flag==1){
				
				int value = ADC_GET_CONVERSION_DATA(ADC, 7);
				if(value<200){
						volume = 0;
				}
				else if(value>3800){
						volume = 100;
				}
				else{
					volume =((value-200)/3600.0)*(100);
				}
				flag = 0;
			}
			if(song ==1){
				PWM_ConfigOutputChannel(PWM0, PWM_CH0, music1[i], volume); // 0=Buzzer ON
				if (music1[i]!=0) PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
				else             PWM_DisableOutput(PWM0, PWM_CH_0_MASK);
				CLK_SysTickDelay(pitch1[i]);	
				if(i==47){
						song=0;
						i=0;
				}
				else{
						i++;
				}
			}
			else if(song ==2){
				PWM_ConfigOutputChannel(PWM0, PWM_CH0, music2[i], volume); // 0=Buzzer ON
				if (music2[i]!=0) PWM_EnableOutput(PWM0, PWM_CH_0_MASK);
				else             PWM_DisableOutput(PWM0, PWM_CH_0_MASK);
				CLK_SysTickDelay(pitch2[i]);	
				if(i==71){
						song=0;
						i=0;
				}
				else{
						i++;
				}
			}
		
		}

}
