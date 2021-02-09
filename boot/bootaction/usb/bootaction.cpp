#include <bsp.hpp>
#include <timer.hpp>
#include <usb.hpp>
#include <wake.hpp>

namespace
{
uint8_t buffer[lib::wake::Protocol::max_frame_size];
}

void boot_action()
{
    using protocol_t = lib::wake::Protocol;
    using packet_t = lib::wake::Packet;
    using timer_t = lib::timer::Timer;

    bsp::usb::init();
    bsp::usb::start_receive(buffer, sizeof(buffer));

    timer_t timer;

    timer.start();

    while(not timer.has_expired(timer_t ::minute(1))) {
        if (bsp::usb::is_idle()) {

            for (std::size_t i = 0; i < bsp::usb::count_receive(); ++i) {
                if (protocol_t::process(buffer[i])) {
                    packet_t packet = protocol_t::unpack();
                }
            }

            bsp::usb::restart_receive();
        }
    }
}
