#include <lib/stream/stream.hpp>
#include <lib/stream/actions.hpp>
#include <cstring>

using namespace lib::stream;

Stream::FormatSettings::FormatSettings()
    : justify(Justify::Left),
      padding_char(' '),
      width(0),
      int_width(),
      radix(10)
{
}

Stream::Stream(Handler& handler, Mode mode)
    : _handler(handler),
      _format_settings(),
      _mode(mode),
      _read_size(),
      _result_read(ResultRead::ERROR)
{
}

Stream::~Stream()
{
}

void Stream::flush()
{
	if (is_text()) operator<<(Endl());
	_handler.flush();
}

void Stream::repeat(uint8_t data, std::size_t count)
{
    for (std::size_t i = 0; i < count; i++)
        write(data);

}

void Stream::write(const void* data, std::size_t size)
{
    if (size == 0) return;

    //O_ASSERT(data != nullptr);
    const uint8_t* cast_data = reinterpret_cast<const uint8_t*>(data);
    
    for(std::size_t i = 0; i < size; i++)
        write(cast_data[i]);
}

void Stream::write(uint8_t data)
{
    handler().write_byte(data);
}

void Stream::read_line(std::string& line)
{
	if (handler().do_read_message(line))
		_result_read = Stream::ResultRead::OK;
	else
		_result_read = Stream::ResultRead::ERROR;
}

void Stream::read_raw(std::vector<uint8_t>& data)
{
	if (handler().do_read_message(data))
		_result_read = Stream::ResultRead::OK;
	else 
		_result_read = Stream::ResultRead::ERROR;
}

Stream& Stream::operator<<(const ActionBase& action)
{
    action.action(*this);
    return *this;
}

Stream& Stream::operator>>(const ActionBase& action)
{
    action.action(*this);
    return *this;
}

Stream& Stream::operator<<(const char* data)
{
    //O_ASSERT(data != nullptr);

    if (is_text())
        *this << StringFormatAction(data);
    else 
        write(data, strlen(data));
    
    return *this;
}

Stream& Stream::operator<<(const std::string& data)
{
    if (not data.empty())
        *this << data.c_str();

    return *this;
}

Stream& Stream::operator<<(std::string_view data)
{
	if (not data.empty())
		*this << data.data();

	return *this;
}

Stream& Stream::operator<<(char data)
{
    write(static_cast<uint8_t>(data));
    return *this;
}

Stream& Stream::operator<<(int64_t data)
{
	if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(uint64_t data)
{
    if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this; 
}

Stream& Stream::operator<<(int32_t data)
{
    if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(uint32_t data)
{
    if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}
    
Stream& Stream::operator<<(int16_t data)
{
    if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(uint16_t data)
{
    if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(int8_t data)
{
    if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(uint8_t data)
{
    if (is_text())
		*this << IntegerFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(const float& data)
{
    if (is_text())
		*this << DoubleFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(const double& data)
{
    if (is_text())
		*this << DoubleFormatAction(data);
	else
		write(&data, sizeof(data));

	return *this;
}

Stream& Stream::operator<<(bool data)
{
	operator<<(static_cast<uint8_t>(data));
	return *this;
}

Stream& Stream::operator>>(std::string& data)
{
	read_line(data);
	return *this;
}

Stream& Stream::operator>>(std::vector<uint8_t>& data)
{
	read_raw(data);
	return *this;
}

Stream& Stream::operator>>(ResultRead& result)
{
	result = _result_read;
	return *this;
}
