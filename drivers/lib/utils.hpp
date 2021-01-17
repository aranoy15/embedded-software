#ifndef DRIVERS_LIB_UTILS
#define DRIVERS_LIB_UTILS

#include <cinttypes>

#define loop while(true)

namespace lib::utils
{
inline uint8_t convert_to_date(const char* p) noexcept
{
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9') v = *p - '0';
	return 10 * v + *++p - '0';
}
}  // namespace lib::utils

#endif /* DRIVERS_LIB_UTILS */
