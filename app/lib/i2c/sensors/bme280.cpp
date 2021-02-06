#include <lib/i2c/sensors/bme280.hpp>
#include <lib/i2c/i2c.hpp>
#include <cstring>

using namespace lib::i2c::sensors;

Bme280::Bme280()
    : _data(), _config(), _measreg(), _humreg(), _timer(), _t_fine(0)
{
}

uint8_t Bme280::chip_id()
{
    return read8(BME280_REGISTER_CHIPID);
}

void Bme280::write8(uint8_t reg, uint8_t value)
{
    uint8_t masterData[] = {reg, value};
    i2c_t::send_reg(_address, reg, value);
}

uint8_t Bme280::read8(uint8_t reg)
{
    i2c_t::send_byte(_address, reg);
    return i2c_t::receive_byte(_address);
}

uint16_t Bme280::read16(uint8_t reg)
{
    uint16_t value;

    i2c_t::send_byte(_address, reg);

    value = i2c_t::receive_byte(_address) << 8;
    value |= i2c_t::receive_byte(_address);

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

    i2c_t::send_byte(_address, reg);

    value = i2c_t::receive_byte(_address);
    value <<= 8;
    value |= i2c_t::receive_byte(_address);
    value <<= 8;
    value |= i2c_t::receive_byte(_address);

    return value;
}

void Bme280::read_coefficients()
{
    _data.dig_T1 = read16_LE(BME280_REGISTER_DIG_T1);
    _data.dig_T2 = readS16_LE(BME280_REGISTER_DIG_T2);
    _data.dig_T3 = readS16_LE(BME280_REGISTER_DIG_T3);

    _data.dig_P1 = read16_LE(BME280_REGISTER_DIG_P1);
    _data.dig_P2 = readS16_LE(BME280_REGISTER_DIG_P2);
    _data.dig_P3 = readS16_LE(BME280_REGISTER_DIG_P3);
    _data.dig_P4 = readS16_LE(BME280_REGISTER_DIG_P4);
    _data.dig_P5 = readS16_LE(BME280_REGISTER_DIG_P5);
    _data.dig_P6 = readS16_LE(BME280_REGISTER_DIG_P6);
    _data.dig_P7 = readS16_LE(BME280_REGISTER_DIG_P7);
    _data.dig_P8 = readS16_LE(BME280_REGISTER_DIG_P8);
    _data.dig_P9 = readS16_LE(BME280_REGISTER_DIG_P9);

    _data.dig_H1 = read8(BME280_REGISTER_DIG_H1);
    _data.dig_H2 = readS16_LE(BME280_REGISTER_DIG_H2);
    _data.dig_H3 = read8(BME280_REGISTER_DIG_H3);
    _data.dig_H4 = (read8(BME280_REGISTER_DIG_H4) << 4) | (read8(BME280_REGISTER_DIG_H4+1) & 0xF);
    _data.dig_H5 = (read8(BME280_REGISTER_DIG_H5+1) << 4) | (read8(BME280_REGISTER_DIG_H5) >> 4);
    _data.dig_H6 = (int8_t)read8(BME280_REGISTER_DIG_H6);
}

bool Bme280::is_reading_calibration()
{
    uint8_t regStatus = read8(BME280_REGISTER_STATUS);
    return (regStatus & (1 << 0)) != 0;
}

void Bme280::set_sampling(SensorMode mode, SensorSampling tempSampling,
                 SensorSampling pressSampling, SensorSampling humSampling,
                 SensorFilter filter, StandbyDuration duration)
{
    _measreg.mode     = mode;
    _measreg.osrs_t   = tempSampling;
    _measreg.osrs_p   = pressSampling;
        
    
    _humreg.osrs_h    = humSampling;
    _config.filter = filter;
    _config.t_sb   = duration;

    
    // you must make sure to also set REGISTER_CONTROL after setting the
    // CONTROLHUMID register, otherwise the values won't be applied (see DS 5.4.3)

    write8(BME280_REGISTER_CONTROLHUMID, _humreg.get());
    write8(BME280_REGISTER_CONFIG, _config.get());
    write8(BME280_REGISTER_CONTROL, _measreg.get());
}

bool Bme280::init()
{
    if (chip_id() != 0x60)
        return false;

    write8(BME280_REGISTER_SOFTRESET, 0xB6);

    using namespace time;

    Time::sleep(Time(300));

    _timer.start(Time::secs(5));
    while(is_reading_calibration()) {
        if (_timer.elapsed())
            return false;

        Time::sleep(Time(100));
    }

    read_coefficients();
    set_sampling();

    Time::sleep(Time(100));

    return true;
}

float Bme280::read_pressure(void) {
    int64_t var1, var2, p;

    read_temperature(); // must be done first to get t_fine

    int32_t adc_P = read24(BME280_REGISTER_PRESSUREDATA);
    if (adc_P == 0x800000) // value in case pressure measurement was disabled
        return -99;
    adc_P >>= 4;

    var1 = ((int64_t)_t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)_data.dig_P6;
    var2 = var2 + ((var1*(int64_t)_data.dig_P5)<<17);
    var2 = var2 + (((int64_t)_data.dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)_data.dig_P3)>>8) +
           ((var1 * (int64_t)_data.dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_data.dig_P1)>>33;

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)_data.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)_data.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)_data.dig_P7)<<4);
    return (float)p/256;
}


float Bme280::read_humidity(void) {
    read_temperature(); // must be done first to get t_fine

    int32_t adc_H = read16(BME280_REGISTER_HUMIDDATA);
    if (adc_H == 0x8000) // value in case humidity measurement was disabled
        return error_value;
        
    int32_t v_x1_u32r;

    v_x1_u32r = (_t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)_data.dig_H4) << 20) -
                    (((int32_t)_data.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)_data.dig_H6)) >> 10) *
                      (((v_x1_u32r * ((int32_t)_data.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)_data.dig_H2) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t)_data.dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    float h = (v_x1_u32r>>12);
    return  h / 1024.0;
}

float Bme280::read_temperature()
{
    int32_t var1, var2;

    int32_t adc_T = read24(BME280_REGISTER_TEMPDATA);
	if (adc_T == 0x800000) // value in case temp measurement was disabled
        return error_value;
    adc_T >>= 4;

    var1 = ((((adc_T>>3) - ((int32_t)_data.dig_T1 <<1))) *
            ((int32_t)_data.dig_T2)) >> 11;
             
    var2 = (((((adc_T>>4) - ((int32_t)_data.dig_T1)) *
              ((adc_T>>4) - ((int32_t)_data.dig_T1))) >> 12) *
            ((int32_t)_data.dig_T3)) >> 14;

    _t_fine = var1 + var2;

    float T = (_t_fine * 5 + 128) >> 8;
    return T/100;
}

void Bme280::take_forced_measurement()
{
    using namespace time;

    if (_measreg.mode == MODE_FORCED) {
        write8(BME280_REGISTER_CONTROL, _measreg.get());
        while(read8(BME280_REGISTER_STATUS) & 0x08)
            Time::sleep(Time(1));
    }
}