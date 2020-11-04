#include <lib/uart/log/log.hpp>
#include <lib/uart/uart.hpp>
#include <bsp.hpp>

using namespace lib::uart::log;
using namespace lib::uart;
using namespace lib::stream;

LogHandler::LogHandler() : StringHandler() {}
LogHandler::~LogHandler() {}

void LogHandler::do_flush()
{
    Uart<bsp::usart::log_port>::send(_data);
    _data.clear();
}

bool LogHandler::do_read_message(std::string& data)
{
    return Uart<bsp::usart::log_port>::read(data);
}

bool LogHandler::do_read_message(std::vector<uint8_t>& data)
{
    return Uart<bsp::usart::log_port>::read(data);
}

Log::Log(uint32_t log_mode)
    : Stream(_handler, Stream::Mode::Text),
      _log_mode(log_mode),
      _handler()
{
}

Log::~Log()
{
    if (is_need_flush()) {
	    flush();
    }
}

void Log::write(uint8_t data)
{
	if ((_log_mode & _log_mode_mask) > 0 or _log_mode == 0) Stream::write(data);
}