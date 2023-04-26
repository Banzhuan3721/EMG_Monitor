#include <msp430f5529.h>
#include <stdint.h>
#include <stdio.h>
#include "sys_clock/SysClock.h"
#include "module/usci_driver/usci_a1_uart/UsciA1Uart.h"

#define NUM_OF_TOTAL_SAMPLED_POINTS 1000

static uint16_t g_TempOfAdc;
static char g_TempOfString[8];

void initAdc12A0(void);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    P2SEL |= BIT2;
    P2DIR |= BIT2;

    configSysMainClock();
    USCI_A1_init();

    initAdc12A0();

    _EINT();

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void SampleFromAdc12A0(void)
{
    ADC12CTL0 |= ADC12ENC;

    ADC12CTL0 |= ADC12SC;
    while((ADC12IFG & BIT0) == 0);
    g_TempOfAdc = ADC12MEM0;

//    USCI_A1_PutChar((g_TempOfAdc & 0xFF00) >> 8);
//    USCI_A1_PutChar((g_TempOfAdc & 0x00FF));

    sprintf(g_TempOfString, "%d\n", g_TempOfAdc);
    USCI_A1_PutString(g_TempOfString);
}

void initAdc12A0(void)
{
    /*选择时钟为 SMCLK 配置 TA0 工作在增计数模式，并清除 TA0R 计数器的初始值*/
    TA0CTL = TASSEL__ACLK + MC__UP + TACLR;
    /*设置计数器计数终值*/
    TA0CCR0 = 64 - 1;
    /*启用 TA0 计数中断*/
    TA0CCTL0 = CCIE;

    REFCTL0 |= REFMSTR; // Reset REFMSTR to hand over control to
    // ADC12_A ref control registers
    /*打开 ADC12 模块，设置采样时间*/
    ADC12CTL0 |= ADC12ON + ADC12SHT0_0;
    /*源自采样定时器，单通道单次转换模式*/
    ADC12CTL1 = ADC12CSTARTADD_0 + ADC12SHS_0 + ADC12SHP + ADC12SSEL_0 + ADC12CONSEQ_0;
    /*设置采样速率为 384Hz*/
    ADC12CTL2 = ADC12RES_2;
    /*配置通道为 A0,VR+ = AVCC = 3.3V, VR- = AVSS*/
    ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_0;
    /*使能 ADC12 的 A0 通道*/
    P6SEL |= BIT0;

}
