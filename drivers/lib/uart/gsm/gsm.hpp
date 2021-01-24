#ifndef DRIVERS_LIB_UART_GPRS_SIM900
#define DRIVERS_LIB_UART_GPRS_SIM900

#include <lib/uart/uart.hpp>
#include <lib/time/timer.hpp>
#include <bsp.hpp>

#if (USE_OS)
#include <lib/os/mutex.hpp>
#endif

namespace lib::uart::gsm
{
class Gsm final
{
public:
    using uart_t = Uart<bsp::usart::gsm_port>;
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
    Gsm() = delete;

    static bool init();
    static void power();

private:
    static bool command(const CommandData& data);

private:

#if (USE_OS)
    inline static lib::os::Mutex _mutex;
#endif
};
}

#endif /* DRIVERS_LIB_UART_GPRS_SIM900 */
