#ifndef DRIVERS_BSP_F103XB_METEOSTATION_USART
#define DRIVERS_BSP_F103XB_METEOSTATION_USART

#include <drivers/bsp/ST/bsp_template.hpp>
#include <cinttypes>

namespace bsp::usart
{
using port_t = bsp::usart::port;

constexpr port_t log_port = port_t::_1;

void init(port_t port);

void send(port_t port, const uint8_t data[], std::size_t size, std::uint32_t timeout);
void send_irq(port_t port, const uint8_t data[], std::size_t size);

bool receive(port_t port, uint8_t data[], std::size_t size, std::uint32_t timeout);
void receive_irq(port_t port, uint8_t data[], std::size_t size);

void stop_receive_irq(port_t port) noexcept;

std::size_t count_receive(port_t port);
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_USART */
