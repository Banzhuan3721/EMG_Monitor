/*
 * USCI_A1_Uart.h
 *
 *  Created on: 2022年4月25日
 *      Author: My
 */

#ifndef USCI_A1_UART_H
#define USCI_A1_UART_H

/*
 * 头文件
 */
#include <msp430f5529.h>
#include <stdio.h>
#include <string.h>
/*
 * 宏定义
 */

/*
 * 函数声明
 */
extern void USCI_A1_init(void);
extern void USCI_A1_PutChar(char Data);
extern char USCI_A1_GetChar(void);
extern void USCI_A1_PutString(char *Chr);



#endif /* USCI_A1_UART_H */
