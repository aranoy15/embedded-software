#pragma once

#include <string>

namespace utils
{
    std::string ftostring(float, uint8_t = 1);

    template <typename T>
    std::string tohex(T data)
    {
	    std::string result = "0x";

        uint8_t* bytes = reinterpret_cast<uint8_t*>(&data);

	    for (uint8_t i = 0; i < sizeof(T); i++) {
		    char hexStr[3];

		    sprintf(hexStr, "%02X", bytes[i]);
		    result += std::string(hexStr);
	    }

	    return result;
    }

	uint8_t conv2d(const char* p); 
}  // namespace utils