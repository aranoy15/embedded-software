#include <lib/stream/handler.hpp>

using namespace lib::stream;

Handler::Handler() : _is_need_flush(false) {}

Handler::~Handler() {}

void Handler::reset()
{
    _is_need_flush = false;
}

void Handler::write_byte(uint8_t data)
{
    do_write_byte(data);
    _is_need_flush = true;
}

void Handler::flush()
{
    _is_need_flush = false;

    before_flush();

    do_flush();
}