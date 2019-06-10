#include <spi.h>

void spi::init(spi::SpiPort port)
{
    switch (port) {
        case spiPort1:
            __HAL_RCC_SPI1_CLK_ENABLE();

            adcSck::setup();
            adcMiso::setup();
            adcMosi::setup();
            adcBusy::setup();

            adcCs::setup();
            adcCs::off();

            break;
    }
}

void spi::chipSelect(spi::SpiPort port)
{
    switch (port) {
        case spiPort1:
            adcCs::on();
            break;
    }
}

void spi::chipUnselect(spi::SpiPort port)
{
    switch (port) {
        case spiPort1:
            adcCs::off();
            break;
    }
}