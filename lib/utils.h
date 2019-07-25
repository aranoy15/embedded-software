#pragma once

#include <string>
#include <stm32f1xx_hal.h>
#include <memory>

enum class DataType
{
    INT = 0,
    HEX = 1,
    BOOL = 2   
};

namespace utils
{
    std::string ftostring(float, uint8_t = 1);
    void delayMicro(uint32_t);

    /*
    template <typename T>
    std::string tohex(T data)
    {
	    std::string result = "0x";

        uint8_t* bytes = reinterpret_cast<uint8_t*>(&data);

	    for (int8_t i = sizeof(T) - 1; i >= 0; i--) {
		    char hexStr[3];

		    sprintf(hexStr, "%02X", bytes[i]);
		    result += std::string(hexStr);
	    }

	    return result;
    }
    */

    template<typename T>
    std::string to_string(T data, DataType type = DataType::INT)
    {
        std::string result;

	    if (type == DataType::HEX) {
		    result = "0x";
		    uint8_t* bytes = reinterpret_cast<uint8_t*>(&data);
		    char temp[10];

		    for (int8_t i = sizeof(T) - 1; i >= 0; i--) {
			    sprintf(temp, "%02X", bytes[i]);
			    result += temp;
		    }
        }
        else if (type == DataType::BOOL) {
            char temp[10];
            sprintf(temp, "%s", static_cast<bool>(data) ? "true" : "false");
            result += temp;
	    } else {
            char temp[20];
            sprintf(temp, "%i", data);
            result += temp;
        }

	    return result;
    }

	uint8_t conv2d(const char* p);

    template <typename... Args>
    std::string stringFormat(const std::string& format, Args... args)
    {
	    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
	    std::unique_ptr<char[]> buf(new char[size]);
	    snprintf(buf.get(), size, format.c_str(), args...);
	    return std::string(buf.get(), buf.get() + size - 1);
    }

#if defined(__arm__)
    extern "C" char* sbrk(int incr);
    static int freeStack()
    {
	    char top = 't';
	    return &top - reinterpret_cast<char*>(sbrk(0));
    }
#endif
}  // namespace utils