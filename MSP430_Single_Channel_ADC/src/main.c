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
    /*ѡ��ʱ��Ϊ SMCLK ���� TA0 ������������ģʽ������� TA0R �������ĳ�ʼֵ*/
    TA0CTL = TASSEL__ACLK + MC__UP + TACLR;
    /*���ü�����������ֵ*/
    TA0CCR0 = 64 - 1;
    /*���� TA0 �����ж�*/
    TA0CCTL0 = CCIE;

    REFCTL0 |= REFMSTR; // Reset REFMSTR to hand over control to
    // ADC12_A ref control registers
    /*�� ADC12 ģ�飬���ò���ʱ��*/
    ADC12CTL0 |= ADC12ON + ADC12SHT0_0;
    /*Դ�Բ�����ʱ������ͨ������ת��ģʽ*/
    ADC12CTL1 = ADC12CSTARTADD_0 + ADC12SHS_0 + ADC12SHP + ADC12SSEL_0 + ADC12CONSEQ_0;
    /*���ò�������Ϊ 384Hz*/
    ADC12CTL2 = ADC12RES_2;
    /*����ͨ��Ϊ A0,VR+ = AVCC = 3.3V, VR- = AVSS*/
    ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_0;
    /*ʹ�� ADC12 �� A0 ͨ��*/
    P6SEL |= BIT0;

}
