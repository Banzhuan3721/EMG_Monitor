/*
 * USCI_A1_Uart.c
 *
 *  Created on: 2022年4月25日
 *      Author: My
 */

/*
 * 头文件
 */
#include "sys_clock/SysClock.h"
#include "module/usci_driver/usci_a1_uart/UsciA1Uart.h"

/******************************************************************************************************
 * 名       称：USCI_A1_init()
 * 功       能：初始化USCI_A1模块为UART模式
 * 入口参数：无
 * 出口参数：无
 * 说       明：UART设为波特率9600,8位数据,无校验,1位停止位
 *
 * 范       例：无
 ******************************************************************************************************/
extern void USCI_A1_init(void)
{
    P4SEL = BIT4 + BIT5;                        // 配置端口P4.4,5 = USCI_A1 TXD/RXD
    P5SEL |= BIT4 + BIT5;                       // Select XT1

    //配置USCI_A1 for UART mode
    UCA1CTL1 |= UCSWRST;                       // 复位USCI_A1
    UCA1CTL1 |= UCSSEL__SMCLK;                  // SMCLK，无校验，8位字符长度，1个停止位
    // Baud Rate计算
    // 32768/(9600) = 3.4133
    // Fractional portion = 0.4133
    // Use Table 24-5 in Family User Guide
    UCA1BR0 = 13;                             //25001984 / (16 * 115200)
    UCA1BR1 = 0x00;
    UCA1MCTL |= UCOS16 | UCBRF_9 | UCBRS_3;                     //0x0300 is UCBRSx = 0x03

    UCA1CTL1 &= ~UCSWRST;                    //启动USCI_A1
    UCA1IE |= UCRXIE;                       //使能 USCI_A1接收中断

}

extern void USCI_A1_PutChar(char Data)
{
   UCA1TXBUF =  Data;
   while(!(UCA1IFG & UCTXIFG));     //等待该字节发送完毕
}

extern char USCI_A1_GetChar(void)
{
    while(!(UCA1IFG & UCRXIFG));    //等待接收1字节
    return (UCA1RXBUF);             //返回接收数据
}

extern void USCI_A1_PutString(char *Chr)
{
    unsigned int i;
    for(i = 0; Chr[i]!='\0';i++)
    {
        UCA1TXBUF = Chr[i];
        while(!(UCA1IFG & UCTXIFG));     //等待该字节发送完毕
    }
}

// /************USCI_A1中断*************/
// #pragma vector=USCI_A1_VECTOR
// __interrupt void USCI_A1_ISR(void)
// {  switch(__even_in_range(UCA1IV,4))
//   {
//   case 0:break;                 //无中断
//   case 2:                       //RXIFG
//       /*在这里写接收中断服务程序代码，如将数据从接收缓冲区读取*/

//     break;
//   case 4:                       //TXIFG
//       /*在这里写发送中断服务程序代码，如将数据压入发送缓冲区*/

//       break;
//   default: break;
//   }
// }
