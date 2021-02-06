#include <lib/uart/log/log.hpp>

using namespace lib::uart::log;
using namespace lib::uart;
using namespace lib::stream;

LogHandler::LogHandler() : StringHandler() {}
LogHandler::~LogHandler() {}

void LogHandler::do_flush()
{
    uart_t::send(_data);
    _data.clear();
}

bool LogHandler::do_read_message(std::string& data)
{
    return uart_t::read(data);
}

bool LogHandler::do_read_message(std::vector<uint8_t>& data)
{
    return uart_t::read(data);
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