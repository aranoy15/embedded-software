#include <bme280.h>
#include <time.h>
#include <utils.h>
#include <cstring>

Bme280::Bme280(uint16_t address)
    : m_data(),
      m_config(),
      m_measreg(),
      m_humreg(),
      m_timer(),
      m_address(address),
      t_fine(0)
{
}

uint8_t Bme280::chipId()
{
    return read8(BME280_REGISTER_CHIPID);
}

void Bme280::write8(uint8_t reg, uint8_t value)
{
    uint8_t masterData[] = {reg, value};
    //getI2C().writeByte(m_address, reg);
    //getI2C().writeByte(m_address, value);
    getI2C().writeReg(m_address, reg, value);
}

uint8_t Bme280::read8(uint8_t reg)
{
    getI2C().writeByte(m_address, reg);
    return getI2C().readByte(m_address);
}

uint16_t Bme280::read16(uint8_t reg)
{
    uint16_t value;

    getI2C().writeByte(m_address, reg);

    value = getI2C().readByte(m_address) << 8;
    value |= getI2C().readByte(m_address);

    return value;
}

uint16_t Bme280::read16_LE(uint8_t reg)
{
    uint16_t value = read16(reg);
    return (value >> 8) | (value << 8);
}

int16_t Bme280::readS16(uint8_t reg)
{
    return (int16_t)read16(reg);
}

int16_t Bme280::readS16_LE(uint8_t reg)
{
    return (int16_t)read16_LE(reg);
}

uint32_t Bme280::read24(uint8_t reg)
{
    uint32_t value; 

    getI2C().writeByte(m_address, reg);

    value = getI2C().readByte(m_address);
    value <<= 8;
    value |= getI2C().readByte(m_address);
    value <<= 8;
    value |= getI2C().readByte(m_address);

    return value;
}

void Bme280::readCoefficients()
{
    m_data.dig_T1 = read16_LE(BME280_REGISTER_DIG_T1);
    m_data.dig_T2 = readS16_LE(BME280_REGISTER_DIG_T2);
    m_data.dig_T3 = readS16_LE(BME280_REGISTER_DIG_T3);

    m_data.dig_P1 = read16_LE(BME280_REGISTER_DIG_P1);
    m_data.dig_P2 = readS16_LE(BME280_REGISTER_DIG_P2);
    m_data.dig_P3 = readS16_LE(BME280_REGISTER_DIG_P3);
    m_data.dig_P4 = readS16_LE(BME280_REGISTER_DIG_P4);
    m_data.dig_P5 = readS16_LE(BME280_REGISTER_DIG_P5);
    m_data.dig_P6 = readS16_LE(BME280_REGISTER_DIG_P6);
    m_data.dig_P7 = readS16_LE(BME280_REGISTER_DIG_P7);
    m_data.dig_P8 = readS16_LE(BME280_REGISTER_DIG_P8);
    m_data.dig_P9 = readS16_LE(BME280_REGISTER_DIG_P9);

    m_data.dig_H1 = read8(BME280_REGISTER_DIG_H1);
    m_data.dig_H2 = readS16_LE(BME280_REGISTER_DIG_H2);
    m_data.dig_H3 = read8(BME280_REGISTER_DIG_H3);
    m_data.dig_H4 = (read8(BME280_REGISTER_DIG_H4) << 4) | (read8(BME280_REGISTER_DIG_H4+1) & 0xF);
    m_data.dig_H5 = (read8(BME280_REGISTER_DIG_H5+1) << 4) | (read8(BME280_REGISTER_DIG_H5) >> 4);
    m_data.dig_H6 = (int8_t)read8(BME280_REGISTER_DIG_H6);
}

bool Bme280::isReadingCalibration()
{
    uint8_t regStatus = read8(BME280_REGISTER_STATUS);
    return (regStatus & (1 << 0)) != 0;
}

void Bme280::setSampling(SensorMode mode, SensorSampling tempSampling,
                 SensorSampling pressSampling, SensorSampling humSampling,
                 SensorFilter filter, StandbyDuration duration)
{
    m_measreg.mode     = mode;
    m_measreg.osrs_t   = tempSampling;
    m_measreg.osrs_p   = pressSampling;
        
    
    m_humreg.osrs_h    = humSampling;
    m_config.filter = filter;
    m_config.t_sb   = duration;

    
    // you must make sure to also set REGISTER_CONTROL after setting the
    // CONTROLHUMID register, otherwise the values won't be applied (see DS 5.4.3)

    write8(BME280_REGISTER_CONTROLHUMID, m_humreg.get());
    write8(BME280_REGISTER_CONFIG, m_config.get());
    write8(BME280_REGISTER_CONTROL, m_measreg.get());
}

bool Bme280::init()
{
    if (chipId() != 0x60)
        return false;

    write8(BME280_REGISTER_SOFTRESET, 0xB6);

    Time::sleep(Time(300));

    m_timer.start(5000);
    while(isReadingCalibration()) {
        if (m_timer.elapsed())
            return false;

        Time::sleep(Time(100));
    }

    readCoefficients();
    setSampling();

    Time::sleep(Time(100));

    return true;
}

float Bme280::readPressure(void) {
    int64_t var1, var2, p;

    readTemperature(); // must be done first to get t_fine

    int32_t adc_P = read24(BME280_REGISTER_PRESSUREDATA);
    if (adc_P == 0x800000) // value in case pressure measurement was disabled
        return -99;
    adc_P >>= 4;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)m_data.dig_P6;
    var2 = var2 + ((var1*(int64_t)m_data.dig_P5)<<17);
    var2 = var2 + (((int64_t)m_data.dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)m_data.dig_P3)>>8) +
           ((var1 * (int64_t)m_data.dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)m_data.dig_P1)>>33;

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)m_data.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)m_data.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)m_data.dig_P7)<<4);
    return (float)p/256;
}


float Bme280::readHumidity(void) {
    readTemperature(); // must be done first to get t_fine

    int32_t adc_H = read16(BME280_REGISTER_HUMIDDATA);
    if (adc_H == 0x8000) // value in case humidity measurement was disabled
        return -99.0f;
        
    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)m_data.dig_H4) << 20) -
                    (((int32_t)m_data.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)m_data.dig_H6)) >> 10) *
                      (((v_x1_u32r * ((int32_t)m_data.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)m_data.dig_H2) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t)m_data.dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    float h = (v_x1_u32r>>12);
    return  h / 1024.0;
}

float Bme280::readTemperature()
{
    int32_t var1, var2;

    int32_t adc_T = read24(BME280_REGISTER_TEMPDATA);
	if (adc_T == 0x800000) // value in case temp measurement was disabled
        return -99.0f;
    adc_T >>= 4;

    var1 = ((((adc_T>>3) - ((int32_t)m_data.dig_T1 <<1))) *
            ((int32_t)m_data.dig_T2)) >> 11;
             
    var2 = (((((adc_T>>4) - ((int32_t)m_data.dig_T1)) *
              ((adc_T>>4) - ((int32_t)m_data.dig_T1))) >> 12) *
            ((int32_t)m_data.dig_T3)) >> 14;

    t_fine = var1 + var2;

    float T = (t_fine * 5 + 128) >> 8;
    return T/100;
}

void Bme280::takeForcedMeasurement()
{
    if (m_measreg.mode == MODE_FORCED) {
        write8(BME280_REGISTER_CONTROL, m_measreg.get());
        while(read8(BME280_REGISTER_STATUS) & 0x08)
            Time::sleep(Time(1));
    }
}