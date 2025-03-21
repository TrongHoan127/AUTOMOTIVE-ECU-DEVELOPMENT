#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define SPI_SCK_Pin GPIO_Pin_0
#define SPI_MISO_Pin GPIO_Pin_1
#define SPI_MOSI_Pin GPIO_Pin_2
#define SPI_CS_Pin GPIO_Pin_3
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA

void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(){
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}

void TIM_Config()
{
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_InitStruct.TIM_Prescaler = 7200 - 1; 
TIM_InitStruct.TIM_Period = 0xFFFF;
TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;    
TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint32_t time)
{
TIM_SetCounter(TIM2, 0);
while (TIM_GetCounter(TIM2) < time * 10) {}
}

void Clock()
{
GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, 1);
delay_ms(4);
GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, 0);
delay_ms(4);
}
void SPI_Init(){
GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}

uint8_t SPI_Slave_Receive(void){
    uint8_t dataReceive = 0x00;
    uint8_t temp;
    
    // Ch? khi CS xu?ng 0
    while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin) == 1);

    for(int i = 0; i < 8; i++){
        // Ch? c?nh l�n c?a clock
        while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin) == 0);

        // �?c d? li?u t? MOSI
        temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);

        // D?ch bit sang tr�i v� ghi gi� tr? v�o dataReceive
        dataReceive = (dataReceive << 1) | temp;

        // Ch? c?nh xu?ng c?a clock
        while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin) == 1);
    }

    // Ch? CS l�n 1 (k?t th�c giao ti?p)
    while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin) == 0);

    return dataReceive;
}
uint8_t DataReceive;
int main()
{

RCC_Config();
GPIO_Config();
TIM_Config();
while(1)
	{
	DataReceive = SPI_Slave_Receive();
		
		}
	}

	
