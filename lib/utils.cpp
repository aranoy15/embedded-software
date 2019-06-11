#include <utils.h>

std::string utils::ftostring(float number, uint8_t fractionDeep)
{
    uint32_t f = 10;

    for (uint8_t i = 0; i < (fractionDeep - 1); i++)
        f *= 10;

    int fraction = (int)((number - (long)number) * f);
    return std::to_string((int)number) + std::string(".") + std::to_string(fraction);
}

uint8_t utils::conv2d(const char *p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

