//
// RTC_LCD_Clock : RTC update a clock on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Scankey.h"
#include "LCD.h"
#include "Seven_Segment.h"

volatile int scount;
uint8_t  comRbuf[256];
int revnumber[4];
volatile uint8_t comRbytes = 0;
volatile uint8_t RX_Flag =0;

void calculate_AB(int number[4], int revnumber[4], int *A, int *B) {
    int used_in_rev[4] = {0}; // ???? revnumber ???????????

    *A = 0;
    *B = 0;

    // ?? A
    for (int i = 0; i < 4; i++) {
        if (number[i] == revnumber[i]) {
            (*A)++;
            used_in_rev[i] = 1; // ??????????
        }
    }

    // ?? B
    for (int i = 0; i < 4; i++) {
        if (number[i] != revnumber[i]) { // ?????? A ???
            for (int j = 0; j < 4; j++) {
                if (i != j && !used_in_rev[j] && number[i] == revnumber[j]) {
                    (*B)++;
                    used_in_rev[j] = 1; // ??????????
                    break; // ??????
                }
            }
        }
    }
}
void Display_7seg(uint32_t value[4])
{
		CloseSevenSegment();
		ShowSevenSegment(3,value[0]);
		CLK_SysTickDelay(5000);
	
		CloseSevenSegment();
		ShowSevenSegment(2,value[1]);
		CLK_SysTickDelay(5000);
		
		CloseSevenSegment();
		ShowSevenSegment(1,value[2]);
		CLK_SysTickDelay(5000);

		CloseSevenSegment();
		ShowSevenSegment(0,value[3]);
		CLK_SysTickDelay(5000);
	
}
int contains(uint32_t arr[], int size, uint32_t value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return 1;
        }
    }
    return 0;
}
void UART02_IRQHandler(void)
{
	uint8_t c, i;
	uint32_t u32IntSts = UART0->ISR;
	PC14 =0;
	

	if(u32IntSts & UART_IS_RX_READY(UART0)) // check ISR on & RX is ready
  {
		while (!(UART0->FSR & UART_FSR_RX_EMPTY_Msk)){ // check RX is not empty
			c = UART_READ(UART0); // read UART RX data
			
			if (c!='\n') {        // check line-end 
				comRbuf[comRbytes] = c;
				comRbytes++;
			} else {
				for (i=0; i<comRbytes; i++)	 revnumber[i]=comRbuf[i]-0x30; // store received data to Message
				comRbytes=0;
				RX_Flag=1;	                 // set flag when BT command input
				break;
			}
		}		
	}
}
void Init_UART(void)
{
	UART_Open(UART0,115200);
  UART_ENABLE_INT(UART0, UART_IER_RDA_IEN_Msk); // enable UART0 interrupt (triggerred by Read-Data-Available)
  NVIC_EnableIRQ(UART02_IRQn);		              // enable Cortex-M0 NVIC interrupt for UART02
}

void SerialPrint(char *Text)
{
 
	UART_Write(UART0, Text, strlen(Text));
	UART_WRITE(UART0, '\n');
	PC13 =0;
}

int main(void)
{	
	uint32_t nowline =0;
	uint32_t i =0;
	uint32_t last_i = 0;
	uint32_t nowpos = 0;
	char enternumber[4];
	uint32_t number[4];
	SYS_Init();
	init_LCD();
	clear_LCD();
	Init_UART();
	OpenSevenSegment();	
	OpenKeyPad();
	srand(scount);
	

	for(int i=0;i<4;i++){
		int newNumber;
		do {
				newNumber = rand() % 9+1; 
		} while (contains(number, i, newNumber));
		number[i] = newNumber;
	}
  while(1) {
		Display_7seg(number);
		i=ScanKey();

    if (RX_Flag==1) {
			if(nowline <4){
				int A = 0, B = 0;
				calculate_AB(number, revnumber, &A, &B);
				char result[20];
				sprintf(result, "%d%d%d%d     %dA%dB", revnumber[0],revnumber[1],revnumber[2],revnumber[3],A, B);
				print_Line(nowline,result);
				nowline++;
			}
			RX_Flag=0;
		}
		if(i!=0){
				last_i=i;
				continue;
		}
		if(last_i>=1&&last_i<=9){
				enternumber[nowpos] = last_i +0x30; 
				last_i = 0;
				if(nowpos ==3){
					nowpos=0;
					SerialPrint(enternumber);
				}
				else{
					nowpos++;

				}
		}

  }
}
