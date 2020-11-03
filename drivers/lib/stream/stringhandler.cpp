#include <drivers/lib/stream/stringhandler.hpp>

using namespace lib::stream;

StringHandler::StringHandler() : Handler(), _data() {}

StringHandler::~StringHandler() {}

void StringHandler::do_write_byte(uint8_t data)
{
    _data += data;
}

StringStream::StringStream() : Stream(_handler, Stream::Mode::Text), _handler()
{
}

StringStream::~StringStream() {}
