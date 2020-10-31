#ifndef DRIVERS_LIB_STREAM_ACTIONS
#define DRIVERS_LIB_STREAM_ACTIONS

#include <lib/stream/actionbase.hpp>
#include <lib/stream/stream.hpp>
#include <inttypes.h>

namespace lib::stream
{

class IntegerFormatAction : public ActionBase
{
    uint64_t _data;
    bool _is_signed;

	void calculate_paddings(const Stream::FormatSettings& settings,
	                        int digital_count, int& left_padding,
	                        int& right_padding) const;

public:
	explicit IntegerFormatAction(uint64_t data);
	explicit IntegerFormatAction(int64_t data);
	explicit IntegerFormatAction(uint32_t data);
	explicit IntegerFormatAction(int32_t data);
	explicit IntegerFormatAction(uint16_t data);
	explicit IntegerFormatAction(int16_t data);
	explicit IntegerFormatAction(uint8_t data);
	explicit IntegerFormatAction(int8_t data);

	virtual void action(Stream& os) const;
};

class DoubleFormatAction : public ActionBase
{
private:
    double _data;
    uint32_t _digits_after_dot_amount;

public:
    explicit DoubleFormatAction(const double& data);
    virtual void action(Stream& stream) const;
    void set_digits_after_dot(const uint32_t digits_after_dot_amount) { _digits_after_dot_amount = digits_after_dot_amount; }
};

class StringFormatAction : public ActionBase
{
private:
    const char* _data;

public:
    StringFormatAction(const StringFormatAction&) = delete;
    StringFormatAction& operator=(const StringFormatAction&) = delete;

    explicit StringFormatAction(const char* data);

    virtual void action(Stream& stream) const;
};

class FlushAction : public ActionBase
{
public:
    FlushAction() = default;

	virtual void action(Stream& stream) const { stream.flush(); }
};

class RadixAction : public ActionBase
{
    int16_t _radix;

public:
	explicit RadixAction(int16_t radix) : _radix(radix) {}

	virtual void action(Stream& stream) const;
};

class WidthAction : public ActionBase
{
	int16_t _width;

public:
	explicit WidthAction(int16_t width) : _width(width){};

	virtual void action(Stream& stream) const;
};


class PaddingCharAction : public ActionBase
{
	char _pedding_char;

public:
	explicit PaddingCharAction(char pedding_char)
	    : _pedding_char(pedding_char){};

	virtual void action(Stream& stream) const;
};


class JustifyAction : public ActionBase
{
	Stream::Justify _justify;

public:
	explicit JustifyAction(Stream::Justify justify) : _justify(justify){};

	virtual void action(Stream& stream) const;
};


class IntWidthAction : public ActionBase
{
	int16_t _width;
	char _pad;

public:
	explicit IntWidthAction(int16_t width, char pad = '0')
	    : _width(width), _pad(pad)
	{
	}

	virtual void action(Stream& stream) const;
};

class ReadSizeAction : public ActionBase
{
	uint16_t _read_size;

public:
	explicit ReadSizeAction(uint16_t read_size) : _read_size(read_size) {}
	virtual void action(Stream& stream) const;
};


class Endl : public ActionBase
{
public:
	Endl(){};

	virtual void action(Stream& os) const
	{
		os << "\n";
	}
};

class Keeper
{
public:
	Keeper(Stream& stream) : _stream(stream), _format_settings(stream.format_settings()) {}
	~Keeper() { _stream.set_format_settings(_format_settings); }

private:
	Stream& _stream;
	Stream::FormatSettings _format_settings;
};
}

#endif /* DRIVERS_LIB_STREAM_ACTIONS */
