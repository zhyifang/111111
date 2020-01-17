/***************************************
 * 文件名  ：usart1.c
 * 描述    ：配置USART1         
 * 实验平台：MINI STM32开发板 基于STM32F103RCT6
 * 硬件连接：------------------------
 *          | PA6  - USART1(Tx)      |
 *          | PA7 -  USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  
 
 
* 文件名  ：usart2.c
 * 描述    ：配置USART1         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：------------------------
 *          | PA2  - USART1(Tx)      |
 *          | PA3- USART1(Rx)      |
 *           ------------------------


**********************************************************************************/

#include "Uart.h"
#include <stdarg.h>
#include "misc.h"


static void NVIC_Configuration()
{
    
    NVIC_InitTypeDef  NVIC_InitStructure;
    
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    
    
}


void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//     NVIC_InitTypeDef  NVIC_InitStructure;
	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置 */    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	USART_Cmd(USART1, ENABLE);// USART1使能
}


void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART2 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	/* USART2 使用IO端口配置 */    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART2 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 9600;//9600  19200   38400    57600 115200，;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART2, &USART_InitStructure);  //初始化USART2
    
    /* USART2 工作模式配置 */
    NVIC_Configuration();
//    USART_ITConfig();
    USART_ITConfig(USART2,  USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);// USART1使能		
}



 /*发送一个字节数据*/
 void UART1SendByte(unsigned char SendData)
{	   
        USART_SendData(USART1,SendData);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  


 void UART2_SendByte(uint8_t SendData)
{	   
        USART_SendData(USART2,SendData);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	    
}  

 /*发送两个字节数据16位*/
void UART1_SendHalfWord(uint16_t data)
{
    uint8_t temp_h ,temp_l;
    temp_h = (data&0xff00)>>8;
    temp_l = data&0xff;
    UART2_SendByte(temp_h);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    UART2_SendByte(temp_l);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
void UART2_SendHalfWord(uint16_t data)
{
    uint8_t temp_h ,temp_l;
    temp_h = (data&0xff00)>>8;
    temp_l = data&0xff;
    UART2_SendByte(temp_h);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    UART2_SendByte(temp_l);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

 /*发送8位数据数组*/

void UART1_SendArray(uint8_t *array ,uint8_t num)
{
    uint8_t i;
    
    for(i=0;i<num;i++)
    {
        
      UART2_SendByte(array[i]);
    
    }
    
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

}
void UART2_SendArray(uint8_t *array ,uint8_t num)
{
    uint8_t i;
    
    for(i=0;i<num;i++)
    {
        
      UART2_SendByte(array[i]);
    
    }
    
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

}

 /*发送字符串*/

void UART1_SendStr(uint8_t *str )
{
    
    uint8_t i=0;
    
    do      
   {    
      UART2_SendByte(*(str+i)); 
      i++;
    }while(*(str+i)!='\0');

  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);


}
void UART2_SendStr(uint8_t *str )
{
    
    uint8_t i=0;
    
    do      
   {    
      UART2_SendByte(*(str+i)); 
      i++;
    }while(*(str+i)!='\0');

  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);


}






/*接收一个字节数据*/
unsigned char UART1GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {  
					return 0;//没有收到数据 
	    	}
        *GetData = USART_ReceiveData(USART1); 
        return 1;//收到数据
}

unsigned char UART2GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        {  
					return 0;//没有收到数据 
		    }
        *GetData = USART_ReceiveData(USART2); 
          return 1;//收到数据
}


 /*发送一个字节数据*/





/*接收一个数据，马上返回接收到的这个数据*/
void UART1Test(void)
{
    unsigned char i = 0;
       while(1)
       {    
		     while(UART1GetByte(&i))
         {
           USART_SendData(USART1,i);
         }      
       }     
}

void UART2Test(void)
{
    unsigned char i = 0;

       while(1)
       {    
		     while(UART2GetByte(&i))
        {
           USART_SendData(USART2,i);
        }      
       }     
}


