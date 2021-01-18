#ifndef DRIVERS_LIB_UART_GPRS_SIM900
#define DRIVERS_LIB_UART_GPRS_SIM900

#include <drivers/lib/uart/uart.hpp>
#include <drivers/lib/time/timer.hpp>
#include <bsp.hpp>

#if (USE_OS)
#include <drivers/lib/os/mutex.hpp>
#endif

namespace lib::uart::gprs
{
class Sim900 final
{
public:
    using uart_t = Uart<bsp::usart::sim900_p>;
    using time_t = lib::time::Time;
    using timer_t = lib::time::Timer;

    inline static const time_t default_timeout = time_t::secs(5);

    enum class DataType
    {
        Command = 0,
        Data = 1
    };

    enum class Protocol
    {
        Closed = 0,
        TCP = 1,
        UDP = 2
    };

public:
    struct CommandData
    {
        std::string_view send;
        std::string_view receive;
        DataType type;
        time_t timeout;

		CommandData()
		    : send(),
		      receive(),
		      type(DataType::Command),
		      timeout(default_timeout)
		{
		}
	};

public:
    Sim900() = delete;

    static bool init();

private:
    static bool command(const CommandData& data);

private:

#if (USE_OS)
    inline static lib::os::Mutex _mutex;
#endif
};
}

#endif /* DRIVERS_LIB_UART_GPRS_SIM900 */
