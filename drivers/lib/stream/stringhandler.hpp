#ifndef DRIVERS_LIB_STREAM_STRINGHANDLER
#define DRIVERS_LIB_STREAM_STRINGHANDLER

#include <string>
#include <drivers/lib/stream/handler.hpp>
#include <drivers/lib/stream/stream.hpp>

namespace lib::stream
{

class StringHandler : public Handler
{
protected:
	std::string _data;

public:
    StringHandler();
    virtual ~StringHandler();

    void do_write_byte(uint8_t data) override;

	std::string& data() { return _data; }
	void clear() { _data.clear(); }
};

class StringStream : public Stream
{
	StringHandler _handler;

public:
	StringStream();
	virtual ~StringStream();

	std::string& data() { return _handler.data(); }
	void clear() { _handler.clear(); }
};

}

#endif /* DRIVERS_LIB_STREAM_STRINGHANDLER */
