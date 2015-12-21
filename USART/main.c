/* USART1 demo with RXNE interrupt
 */
#include "stm32f4xx.h"

void RCC_Config()
{
	/* USART1 Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
	/* GPIO Clock */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG, ENABLE );
}

void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/* USART1:
	 * TX: PA9, Connect to RX of USB-TTL.
	 * RX: PA10, COnnect to TX of USB-TTL. */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStruct );

	/* Select USART1 as alternative function for PA9 and PA10 */
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource9, GPIO_AF_USART1 );
	GPIO_PinAFConfig( GPIOA, GPIO_PinSource10, GPIO_AF_USART1 );
}

void USART1_Config()
{
	/* Config USART1 */
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init( USART1, &USART_InitStruct );

	/* Enable RXNE interrupt */
	USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStruct );

	USART_Cmd( USART1, ENABLE );
}

void USART1_puts( volatile char *s )
{
	while (*s) {
		while( USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET )
			;	// Wait until TX is empty;
		USART_SendData( USART1, *s );
		++s;
	}
}

void USART1_putc( volatile char c )
{
	char s[2];
	s[0] = c; s[1] = '\0';
	USART1_puts( s );
}

int main()
{
	RCC_Config();
	GPIO_Config();
	USART1_Config();

	while (1) {
			;
	}
}

/* Interrupt request handler of USART1
 */
void USART1_IRQHandler()
{
	if ( USART_GetITStatus( USART1, USART_IT_RXNE ) == SET ) {
		USART_ClearFlag( USART1, USART_IT_RXNE );
		char t = USART1->DR;
		USART1_putc( t );
	}
}
