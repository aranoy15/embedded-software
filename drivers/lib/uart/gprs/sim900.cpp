#include <drivers/lib/uart/gprs/sim900.hpp>

using namespace lib::uart::gprs;

bool Sim900::init()
{
    CommandData data;

    data.send = "AT\r\n";
    data.receive = "OK";
    data.type = DataType::Command;
    data.timeout = time_t::mins(1);

    if (not command(data)) return false;

    return true;
}

bool Sim900::command(const CommandData& data)
{

    uart_t::send(data.send);

    timer_t timer;
    std::string receive_data;

    timer.start(data.timeout);

    while (not timer.elapsed()) {
        if (uart_t::read(receive_data)) {
            if (receive_data == data.receive) return true;
        }

        time_t::sleep(time_t::msecs(10));
    }

    return false;
}
