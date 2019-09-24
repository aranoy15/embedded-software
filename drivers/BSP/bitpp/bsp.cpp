#include <bsp.h>

void bsp::cpuInit()
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	__enable_irq();
	
	//__HAL_RCC_GPIOA_CLK_ENABLE();

	__HAL_RCC_AFIO_CLK_ENABLE();
  	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	//RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	O_ASSERT(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) == HAL_OK);

}

//void bsp::uart::usartInitGpio(bsp::UartPort port)
//{
//	switch (port) {
//		case bsp::uartP1:
//			if (__HAL_RCC_USART1_IS_CLK_DISABLED())
//				__HAL_RCC_USART1_CLK_ENABLE();
//
//			uart::usart1RxPin::setup();
//			uart::usart1TxPin::setup();
//
//			HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//			HAL_NVIC_EnableIRQ(USART1_IRQn);
//
//			break;
//	}
//}

//USART_TypeDef* bsp::uart::port2CSP(bsp::UartPort port)
//{
//	switch (port) {
//		case bsp::uartP1:
//			return USART1;
//	}
//
//	return nullptr;
//}

void bsp::usb::init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USB_CLK_ENABLE();

	HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

void bsp::usb::deinit()
{
	__HAL_RCC_USB_CLK_DISABLE();

	HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
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

            adcFlashSck::setup();
            adcFlashMiso::setup();
            adcFlashMosi::setup();

            break;
		case spiP2:
			__HAL_RCC_SPI2_CLK_ENABLE();

			sdioSck::setup();
			sdioMiso::setup();
			sdioMosi::setup();

			sdio::sdioCd::setup();

			HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  			HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

			break;
    }
}

void bsp::spi::chipSelect(uint8_t type)
{
    switch (type) {
        case AnalogInput:
            adc::adcCs::on();
            break;
		case Flash:
			flash::flashCs::on();
			break;
		case Sdio:
			sdio::sdioCs::on();
			break;
    }
}

void bsp::spi::chipUnselect(uint8_t type)
{
    switch (type) {
        case AnalogInput:
            adc::adcCs::off();
            break;
		case Flash:
			flash::flashCs::off();
			break;
		case Sdio:
			sdio::sdioCs::off();
			break;
    }
}

void bsp::flash::init()
{
	flashCs::setup();
	flashCs::off();
}


void bsp::adc::init()
{
	adcBusy::setup();

	adcCs::setup();
	adcCs::off();
}


void bsp::sdio::init()
{
	sdioCd::setup();

	sdioCs::setup();
	sdioCs::off();
}

void bsp::rtc::init()
{
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_BKP_CLK_ENABLE();
	__HAL_RCC_RTC_ENABLE();
}

void bsp::rtc::deinit()
{
	__HAL_RCC_RTC_DISABLE();
}