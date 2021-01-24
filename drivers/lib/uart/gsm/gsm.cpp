#include <lib/uart/gsm/gsm.hpp>
#include <lib/uart/log/log.hpp>

using namespace lib::uart::gsm;

bool Gsm::init()
{
    bsp::gpio::gsm::off();

    CommandData data;

    data.send = "AT\r\n";
    data.receive = "OK";
    data.type = DataType::Command;
    data.timeout = time_t::mins(1);

    if (not command(data)) return false;

    return true;
}

void Gsm::reset()
{
    bsp::gpio::gsm::off();
    time_t::sleep(time_t::secs(1));
    bsp::gpio::gsm::on();
    time_t::sleep(time_t::secs(1));
    bsp::gpio::gsm::off();
    time_t::sleep(time_t::secs(1));
}

bool Gsm::command(const CommandData& data)
{
    using namespace lib::uart::log;
    using namespace lib::stream;

    uart_t::clear();

    uart_t::send(data.send);

    timer_t timer;
    std::string receive_data;

    timer.start(data.timeout);

    while (not timer.elapsed()) {
        if (uart_t::read(receive_data)) {
            Log() << "receive data: " << receive_data << Endl();

            if (receive_data == data.receive) { 
                Log() << "return true" << Endl();
                return true;
            }

            receive_data.clear();
        }

        time_t::sleep(time_t::msecs(10));
    }

    return false;
}
