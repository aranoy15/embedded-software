#ifndef DRIVERS_LIB_UART_UART
#define DRIVERS_LIB_UART_UART

#include <bsp.hpp>
#include <string>
#include <vector>
#include <drivers/lib/circular_buffer.h>

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

    static void receive_complete() noexcept;
    static void idle() noexcept;

    static bool read(std::string& data) noexcept;
    static bool read(std::vector<uint8_t>& data) noexcept;
    static bool read(uint8_t data[], std::size_t size) noexcept;

private:
    static const uint32_t _timeout = 1000;
    inline static uint8_t _temp[temp_size];
    inline static circular_buffer<uint8_t, buffer_size> _data;
};

template<port_t port>
void Uart<port>::init() noexcept
{
    bsp::usart::init(port);

    bsp::usart::receive_irq(port, _temp, temp_size);
}

template<port_t port>
void Uart<port>::send(uint8_t data[], std::size_t size) noexcept
{
    bsp::usart::send(port, data, size, _timeout);
}

template<port_t port>
void Uart<port>::send(const std::string& data) noexcept
{
    bsp::usart::send(port, reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())), data.size(), _timeout);
}

template<port_t port>
bool Uart<port>::read(std::string& data) noexcept
{
    if (_data.empty()) return false;

    while (not _data.empty()) {
        uint8_t b = _data.get();

        if (b == '\n') break;

        data.push_back(b);
    }

    return true;
}

template<port_t port>
bool Uart<port>::read(uint8_t data[], std::size_t size) noexcept
{
    if (_data.size() < size) return false;

    for (std::size_t i = 0; i < size; ++i) {
        data[i] = _data.get();
    }

    return true;
}

template<port_t port>
bool Uart<port>::read(std::vector<uint8_t>& data) noexcept
{
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
    bsp::usart::receive_irq(port, _temp, temp_size);
}

template<port_t port>
void Uart<port>::idle() noexcept
{
    for (std::size_t i = 0; i < bsp::usart::count_receive(port); i++) {
        _data.put(_temp[i]);
    }

    bsp::usart::stop_receive_irq(port);
    bsp::usart::receive_irq(port, _temp, temp_size);
}

}

#endif /* DRIVERS_LIB_UART_UART */
