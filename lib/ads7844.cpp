#include <ads7844.h>
#include <utils.h>
#include <time.h>

const uint8_t Ads7844::channels[channelCount] = {
    0 << shift,
    4 << shift,
    1 << shift,
    5 << shift,
    2 << shift,
    6 << shift,
    3 << shift,
    7 << shift
};

Ads7844::Ads7844() : m_timer()
{
    bsp::adc::init();
}

uint16_t Ads7844::getValue(uint8_t channel)
{
    if (channel > 7) channel = 7;

    uint16_t result = 0;

    {
        adcChipSelectTrigger acsTrigger(bsp::spi::AnalogInput);
        utils::delayMicro(1);
        
        getSpi().sendByte(cbStart | channels[channel] | cbSGLDIF | cbPD1 | cbPD0);

        m_timer.start(2000);
        while(not bsp::adc::adcBusy::state()){
            if(m_timer.elapsed())
                return result;

            Time::sleep(Time(5));
        }

		uint8_t resultH = getSpi().receiveByte();
        uint8_t resultL = getSpi().receiveByte();

        result = ((resultH << 8) | resultL) >> 3;

        //adcResult = (12.0f * intResult) / 0xFFF;
    }

    Time::sleep(Time(100));

    return result;
}

float Ads7844::calcValue(uint16_t value) 
{
    return utils::map(value, 0, 0xFFF, 0, 12.0f);
}