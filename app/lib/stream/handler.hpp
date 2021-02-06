#ifndef DRIVERS_LIB_STREAM_STREAMHANDLER
#define DRIVERS_LIB_STREAM_STREAMHANDLER

#include <inttypes.h>
#include <vector>
#include <string>

namespace lib::stream
{

class Handler
{

	bool _is_need_flush;

private:
    void reset();

public:
    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;

    Handler();
    virtual ~Handler();

    void write_byte(uint8_t data);
    uint8_t read_byte();
    uint16_t data_count();

    void flush();
	bool is_need_flush() const { return _is_need_flush; }

	virtual void do_write_byte(uint8_t data) = 0;

    virtual bool do_read_message(std::string&) { return false; }
    virtual bool do_read_message(std::vector<uint8_t>&) { return false; }

    virtual void do_flush() {}

    virtual void before_flush() {}
};
}

#endif /* DRIVERS_LIB_STREAM_STREAMHANDLER */
