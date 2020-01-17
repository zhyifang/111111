#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>


void USART1_Config(void);
void USART2_Config(void);

void UART1Test(void);
void UART2Test(void);
void UART1_SendByte(uint8_t SendData);
void UART2_SendByte(uint8_t SendData);
 
void UART1_SendHalfWord(uint16_t data);
void UART2_SendHalfWord(uint16_t data);

void UART1_SendArray(uint8_t *array ,uint8_t num);
void UART2_SendArray(uint8_t *array ,uint8_t num);

void UART1_SendStr(uint8_t *str );
void UART2_SendStr(uint8_t *str );
#endif /* __USART1_H */
