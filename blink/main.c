#include "stm32f4xx.h"

int main()
{
	// All GPIO Pins are on AHB1 bus.
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE );

	// Initialize GPIO:
	// PG13, PG14: On board LED.
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_init.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_init.GPIO_OType = GPIO_OType_PP;
	GPIO_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOG, &GPIO_init );

	while(1) {
		GPIO_SetBits( GPIOG, GPIO_Pin_13 );
		GPIO_ResetBits( GPIOG, GPIO_Pin_14 );

		for ( uint32_t i = 0; i < 1000000; ++i )
			;

		GPIO_SetBits( GPIOG, GPIO_Pin_14 );
		GPIO_ResetBits( GPIOG, GPIO_Pin_13 );

		for ( uint32_t i = 0; i < 1000000; ++i )
			;
	}

	return 0;
}
