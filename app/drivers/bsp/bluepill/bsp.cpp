#include <bsp.h>

void bsp::cpuInit()
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__HAL_RCC_AFIO_CLK_ENABLE();
  	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	O_ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK);
	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
	                              RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	O_ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) == HAL_OK);
}

void bsp::uart::usartInitGpio(bsp::UartPort port)
{
	switch (port) {
		case bsp::uartP1:
			if (__HAL_RCC_USART1_IS_CLK_DISABLED())
				__HAL_RCC_USART1_CLK_ENABLE();

			uart::usart1RxPin::setup();
			uart::usart1TxPin::setup();

			HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
			HAL_NVIC_EnableIRQ(USART1_IRQn);

			break;

		case bsp::uartP2:
			if (__HAL_RCC_USART2_IS_CLK_DISABLED())
				__HAL_RCC_USART2_CLK_ENABLE();

			uart::usart2RxPin::setup();
			uart::usart2TxPin::setup();

			HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
			HAL_NVIC_EnableIRQ(USART2_IRQn);

			break;
	}
}

USART_TypeDef* bsp::uart::port2CSP(bsp::UartPort port)
{
	switch (port) {
		case bsp::uartP1:
			return USART1;
		case bsp::uartP2:
			return USART2;
		case bsp::uartP3:
			return USART3;
	}

	return nullptr;
}

void bsp::i2c::init(bsp::I2CPort port)
{
	switch (port) {
		case bsp::i2cP1:
			if (__HAL_RCC_I2C1_IS_CLK_DISABLED()) __HAL_RCC_I2C1_CLK_ENABLE();

			i2c::i2cPins::setup();
			break;
	}
}

I2C_TypeDef* bsp::i2c::port2CSP(bsp::I2CPort port)
{
	if (port == bsp::i2cP1) return I2C1;

	return I2C2;
}

SPI_TypeDef* bsp::spi::port2CSP(bsp::SpiPort port)
	{
		switch (port) {
			default:
			case bsp::spiP1:
				return SPI1;
			case bsp::spiP2:
				return SPI2;
		}
	}

void bsp::spi::init(bsp::SpiPort port)
{
    switch (port) {
        case spiP1:
            __HAL_RCC_SPI1_CLK_ENABLE();

            adcSck::setup();
            adcMiso::setup();
            adcMosi::setup();

            adcCs::setup();
            adcCs::off();

            break;
    }
}

void bsp::spi::chipSelect(bsp::SpiPort port)
{
    switch (port) {
        case spiP1:
            adcCs::on();
            break;
    }
}

void bsp::spi::chipUnselect(bsp::SpiPort port)
{
    switch (port) {
        case spiP1:
            adcCs::off();
            break;
    }
}