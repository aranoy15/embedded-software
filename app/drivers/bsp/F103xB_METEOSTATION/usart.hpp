#ifndef DRIVERS_BSP_F103XB_METEOSTATION_USART
#define DRIVERS_BSP_F103XB_METEOSTATION_USART

#include <bsp_template.hpp>
#include <cinttypes>

namespace bsp::usart
{
using port_t = bsp::usart::port;

constexpr port_t log_port = port_t::_1;
constexpr port_t mhz_port = port_t::_2;

void init(port_t port);

void send(port_t port, const uint8_t data[], std::size_t size);

bool start_receive(port_t port, uint8_t data[], std::size_t size) noexcept;
bool stop_receive(port_t port) noexcept;
bool restart_receive(port_t port, uint8_t data[], std::size_t size) noexcept;


std::size_t count_receive(port_t port);
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_USART */
