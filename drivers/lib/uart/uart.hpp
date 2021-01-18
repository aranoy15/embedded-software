#ifndef DRIVERS_LIB_UART_UART
#define DRIVERS_LIB_UART_UART

#include <bsp.hpp>
#include <string>
#include <vector>
#include <drivers/lib/circular_buffer.h>

#if (USE_OS)
#include <drivers/lib/lock.hpp>
#include <drivers/lib/os/mutex.hpp>
#endif

namespace lib::uart
{
using port_t = bsp::usart::port;

constexpr std::size_t temp_size = 64;
constexpr std::size_t buffer_size = 512;

template<port_t port>
struct Uart
{
    Uart() = delete;

    static void init() noexcept;

    static void send(uint8_t data[], std::size_t size) noexcept;
    static void send(const std::string& data) noexcept;
    static void send(std::string_view data) noexcept;

    static void receive_complete() noexcept;
    static void transmit_complete() noexcept;
    static void idle() noexcept;

    static bool read(std::string& data) noexcept;
    static bool read(std::vector<uint8_t>& data) noexcept;
    static bool read(uint8_t data[], std::size_t size) noexcept;

    static void clear() { _data.reset(); }

private:
    inline static uint8_t _temp[temp_size];
    inline static circular_buffer<uint8_t, buffer_size> _data;

#if (USE_OS)
    inline static lib::os::Mutex _send_mutex;
    inline static lib::os::Mutex _receive_mutex;
#endif
};

template<port_t port>
void Uart<port>::init() noexcept
{
    bsp::usart::init(port);

    bsp::usart::start_receive(port, _temp, temp_size);
}

template<port_t port>
void Uart<port>::send(uint8_t data[], std::size_t size) noexcept
{
#if (USE_OS)
    lib::Lock lock(_send_mutex);
#endif

    bsp::usart::send(port, data, size);
}

template<port_t port>
void Uart<port>::send(const std::string& data) noexcept
{
#if (USE_OS)
    lib::Lock lock(_send_mutex);
#endif

    bsp::usart::send(port, reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())), data.size());
}

template<port_t port>
void Uart<port>::send(std::string_view data) noexcept
{
#if (USE_OS)
    lib::Lock lock(_send_mutex);
#endif

    bsp::usart::send(port, reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())), data.size());
}

template<port_t port>
bool Uart<port>::read(std::string& data) noexcept
{
#if (USE_OS)
    lib::Lock lock(_receive_mutex);
#endif

    if (_data.empty()) return false;

    while (not _data.empty()) {
        uint8_t b = _data.get();

        if (b == '\n') return true;

        if (b != '\r') data.push_back(b);
    }

    return false;
}

template<port_t port>
bool Uart<port>::read(uint8_t data[], std::size_t size) noexcept
{
#if (USE_OS)
    lib::Lock lock(_receive_mutex);
#endif

    if (_data.size() < size) return false;

    for (std::size_t i = 0; i < size; ++i) {
        data[i] = _data.get();
    }

    return true;
}

template<port_t port>
bool Uart<port>::read(std::vector<uint8_t>& data) noexcept
{
#if (USE_OS)
    lib::Lock lock(_receive_mutex);
#endif

    std::size_t size = data.capacity();
	if (_data.size() < size or data.capacity() == 0) return false;

    for (std::size_t i = 0; i < size; ++i) {
        data.push_back(_data.get());
    }

    return true;
}

template<port_t port>
void Uart<port>::receive_complete() noexcept
{
    bsp::usart::restart_receive(port, _temp, temp_size);
}

template<port_t port>
void Uart<port>::transmit_complete() noexcept
{
}

template<port_t port>
void Uart<port>::idle() noexcept
{
    for (std::size_t i = 0; i < bsp::usart::count_receive(port); i++) {
        _data.put(_temp[i]);
    }

    bsp::usart::restart_receive(port, _temp, temp_size);
}

}

#endif /* DRIVERS_LIB_UART_UART */
