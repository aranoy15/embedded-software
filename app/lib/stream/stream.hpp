#ifndef DRIVERS_LIB_STREAM_STREAM
#define DRIVERS_LIB_STREAM_STREAM

#include <lib/stream/handler.hpp>
#include <lib/stream/actionbase.hpp>
#include <string>
#include <vector>
#include <array>
#include <inttypes.h>

namespace lib::stream
{

class Stream
{
public:
	enum class Justify
	{
		Left = 1,
		Center,
		Right
	};

    enum class Mode 
    {
        Binary,
        Text
    };

    enum class ResultRead
    {
        ERROR = 0,
        OK = 1
    };

    struct FormatSettings
    {
        Justify justify;
        char padding_char;
        int16_t width;
        int16_t int_width;
        int16_t radix;

	public:
		FormatSettings();
	};

	struct ReadSize
	{
		uint16_t data_size;

	public:
		ReadSize() : data_size(0) {}
	};

	class MessageOverflowEx : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Message read overflow exception";
		}
	};

private:
    Handler& _handler;
    FormatSettings _format_settings;
    Mode _mode;
    ReadSize _read_size;
    ResultRead _result_read;

private:
    void read_line(std::string& line);
    void read_raw(std::vector<uint8_t>& data);

public:
    Stream(const Stream&) = delete;
    Stream operator=(const Stream&) = delete;

    explicit Stream(Handler& handler, Mode mode);
    virtual ~Stream();

	Mode mode() const { return _mode; }
	void set_mode(Mode mode) { _mode = mode; }

	bool is_text() const { return _mode == Mode::Text; }
	bool is_binary() const { return _mode == Mode::Binary; }

	Handler& handler() const { return _handler; }
	bool is_need_flush() const { return _handler.is_need_flush(); }
	void flush(); 

	const Stream::FormatSettings& format_settings() const { return _format_settings; }
    void set_format_settings(const FormatSettings& settings) { _format_settings = settings; }

	const Stream::ReadSize& read_size() const { return _read_size; }
    void set_read_size(const Stream::ReadSize& read_size) { _read_size = read_size; }

    void repeat(uint8_t data, std::size_t count);
	void repeat(char data, std::size_t count) { repeat(static_cast<uint8_t>(data), count); }

    void write(const void* data, std::size_t size);
    virtual void write(uint8_t data);

    Stream& operator<<(const ActionBase& action);
    Stream& operator<<(const char* data);
    Stream& operator<<(const std::string& data);
    Stream& operator<<(std::string_view data);
    Stream& operator<<(char data);

    Stream& operator<<(int64_t data);
    Stream& operator<<(uint64_t data);

    Stream& operator<<(int32_t data);
    Stream& operator<<(uint32_t data);
    
    Stream& operator<<(int16_t data);
    Stream& operator<<(uint16_t data);

    Stream& operator<<(int8_t data);
    Stream& operator<<(uint8_t data);

    Stream& operator<<(const float& data);
    Stream& operator<<(const double& data);

    Stream& operator<<(bool data);

    Stream& operator>>(const ActionBase& action);

    Stream& operator>>(std::string& data);
    Stream& operator>>(std::vector<uint8_t>& data);
    Stream& operator>>(ResultRead& result);

};
}

#endif /* DRIVERS_LIB_STREAM_STREAM */
