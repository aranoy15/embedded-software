#pragma once

#include <string>
#include <lib/stream/stringhandler.hpp>
#include <lib/stream/stream.hpp>
#include <lib/stream/actions.hpp>
#include <lib/uart/uart.hpp>
#include <bsp.hpp>

namespace lib::uart::log
{

class LogHandler : public stream::StringHandler
{
public:
    using uart_t = Uart<bsp::usart::log_port>;

public:
    LogHandler(); 

    virtual ~LogHandler();

    bool do_read_message(std::string& data) override;
    bool do_read_message(std::vector<uint8_t>& data) override;

    void do_flush() override;
};

class Log : public stream::Stream
{
private:
    inline static uint32_t _log_mode_mask = 0;
    inline static uint32_t _request_mask = 0;

private:
    uint32_t _log_mode;
    LogHandler _handler;

public:
    Log(uint32_t log_mode = 0);
    virtual ~Log();

    void write(uint8_t data) override;

	static void set_log_mask(uint32_t log_mask) { _log_mode_mask = log_mask; }
	static void include_log_mask(uint32_t mask) { _log_mode_mask |= mask; }
	static void remove_log_mask(uint32_t mask) { _log_mode_mask &= ~mask; }

    static bool have_request(uint32_t request) { return request == (_request_mask & request); }
    static void set_request_mask(uint32_t mask) { _request_mask = mask; }
    static uint32_t request_mask() { return _request_mask; }
    static void complete_request(uint32_t mask) { _request_mask &= ~mask; }
};

}
