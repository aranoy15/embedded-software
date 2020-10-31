#include <lib/stream/actions.hpp>
#include <cstring>

namespace
{
char digital_char_by_index(int index)
{
    static const char* digits = "0123456789ABCDEF";
	//O_ASSERT(index < 16);
	return digits[index];
}
}

using namespace lib::stream;

IntegerFormatAction::IntegerFormatAction(uint64_t data)
    : _data(data), _is_signed(false)
{
}

IntegerFormatAction::IntegerFormatAction(int64_t data)
    : _data(data), _is_signed(true)
{
}

IntegerFormatAction::IntegerFormatAction(uint32_t data)
    : _data(data), _is_signed(false)
{
}

IntegerFormatAction::IntegerFormatAction(int32_t data)
    : _data(data), _is_signed(true)
{
}

IntegerFormatAction::IntegerFormatAction(uint16_t data)
    : _data(data), _is_signed(false)
{
}

IntegerFormatAction::IntegerFormatAction(int16_t data)
    : _data(data), _is_signed(true)
{
}

IntegerFormatAction::IntegerFormatAction(uint8_t data)
    : _data(data), _is_signed(false)
{
}

IntegerFormatAction::IntegerFormatAction(int8_t data)
    : _data(data), _is_signed(true)
{
}

/*
IntegerFormatAction::IntegerFormatAction(int data)
    : _data(data), _is_signed(true)
{
}

IntegerFormatAction::IntegerFormatAction(unsigned data)
    : _data(data), _is_signed(false)
{
}

IntegerFormatAction::IntegerFormatAction(std::size_t data)
    : _data(data), _is_signed(false)
{
}
*/

void IntegerFormatAction::action(Stream& stream) const
{
    auto& format_settings = stream.format_settings();
    //O_ASSERT(format_settings.radix <= 16);

    const int max_digits = 32;
    char digits[max_digits] = {0};

    bool is_negative = _is_signed and (static_cast<int32_t>(_data) < 0);

    uint64_t number = _is_signed ? abs(static_cast<int64_t>(_data)) : _data;
    uint64_t devider = format_settings.radix;
    int digital_count = 0;

    do {
        //O_ASSERT(digital_count < max_digits);
        int digit = number % devider;
        digits[max_digits - digital_count - 1] = digital_char_by_index(digit);

        number /= devider;
        ++digital_count;
    } while (number != 0);

	if (is_negative) {
		//O_ASSERT(digital_count < max_digits);
		digits[max_digits - digital_count - 1] = '-';
		++digital_count;
	}

    int left_padding = 0;
    int right_padding = 0;
    calculate_paddings(format_settings, digital_count, left_padding, right_padding);

    const uint8_t* write_data = reinterpret_cast<const uint8_t*>(digits + max_digits - digital_count);

    stream.repeat(format_settings.padding_char, left_padding);
    stream.write(write_data, digital_count);
    stream.repeat(format_settings.padding_char, right_padding);
}

void IntegerFormatAction::calculate_paddings(
    const Stream::FormatSettings& settings, int digital_count,
    int& left_padding, int& right_padding) const
{
	if (settings.int_width == 0) {
        left_padding = 0;
        right_padding = 0;
        return;
    }

    switch (settings.justify) {
        case Stream::Justify::Left:
            left_padding = 0;
            right_padding = settings.int_width - digital_count;
            break;
        
        case Stream::Justify::Center:
            left_padding = (settings.int_width - digital_count) / 2;
            right_padding = settings.int_width - left_padding;
            break;
        
        case Stream::Justify::Right:
            left_padding = settings.int_width - digital_count;
            right_padding = 0;
            break;
    }

    //O_ASSERT(left_padding >= 0);
    //O_ASSERT(right_padding >= 0);
}

DoubleFormatAction::DoubleFormatAction(const double& data)
    : _data(data), _digits_after_dot_amount(2)
{
}

void DoubleFormatAction::action(Stream& stream) const
{
    double data = _data;
    const uint32_t max_digits_after_dot = 12;

    char digits_after_dot[max_digits_after_dot] = {0};

    if (data < 0) data *= -1;

    data -= static_cast<int32_t>(data);

    for (uint32_t i = 0; i < _digits_after_dot_amount; i++) {
        data = data * 10;
        int32_t digit = static_cast<int32_t>(data);

        //O_ASSERT(digit <= 9);

        data -= digit;
        digits_after_dot[i] = '0' + digit;
    }

    int32_t integer_part = static_cast<int32_t>(_data);
    uint32_t number = (integer_part < 0) ? abs(integer_part) : integer_part;

    const uint32_t devider = 10;
    uint32_t digit_count = 0;

    const uint32_t max_digits_before_dot = 24;
    char digits_before_dot[max_digits_before_dot] = {0};

    do {
        //O_ASSERT(digit_count < max_digits_before_dot);
        int digit = number % devider;
        digits_before_dot[max_digits_before_dot - digit_count - 1] = '0' + digit;

        number /= devider;
        ++digit_count;
    } while (number != 0);

    if (_data < 0) {
        //O_ASSERT(digit_count < max_digits_before_dot);
        digits_before_dot[max_digits_before_dot - digit_count - 1] = '-';
        ++digit_count;
    }

    const uint32_t max_digits = max_digits_after_dot + max_digits_before_dot;
    char digits[max_digits] = {0};

    memcpy(digits, digits_before_dot + (max_digits_before_dot - digit_count), digit_count);
    memcpy(digits + digit_count, ".", 1);

    digit_count++;
    memcpy(digits + digit_count, digits_after_dot, _digits_after_dot_amount);

    const uint8_t* write_data = reinterpret_cast<const uint8_t*>(digits);
    stream.write(write_data, _digits_after_dot_amount + digit_count);
}

StringFormatAction::StringFormatAction(const char* data) : _data(data) {}

void StringFormatAction::action(Stream& stream) const
{
    auto& format_settings = stream.format_settings();

    int32_t size = strlen(_data);
    uint16_t left_padding = 0;
    uint16_t right_padding = 0;

    if (format_settings.width > size) {
        switch (format_settings.justify) {
            case Stream::Justify::Left:
                left_padding = 0;
                right_padding = format_settings.width - size;
                break;
            
            case Stream::Justify::Center:
                left_padding = (format_settings.width - size) / 2;
                right_padding = (format_settings.width - size) - left_padding;
                break;
            
            case Stream::Justify::Right:
                left_padding = format_settings.width - size;
                right_padding = 0;
                break;
        }
    }

    const uint8_t* write_data = reinterpret_cast<const uint8_t*>(_data);

    stream.repeat(format_settings.padding_char, left_padding);
    stream.write(write_data, size);
    stream.repeat(format_settings.padding_char, right_padding);
}

void RadixAction::action(Stream& stream) const
{
	auto& format_settings = const_cast<Stream::FormatSettings&>(stream.format_settings());
	format_settings.radix = _radix;
	stream.set_format_settings(format_settings);
}

void WidthAction::action(Stream& stream) const
{
	auto& format_settings = const_cast<Stream::FormatSettings&>(stream.format_settings());
	format_settings.width = _width;
	stream.set_format_settings(format_settings);
}

void PaddingCharAction::action(Stream& stream) const
{
	auto& format_settings = const_cast<Stream::FormatSettings&>(stream.format_settings());
	format_settings.padding_char = _pedding_char;
	stream.set_format_settings(format_settings);
}

void JustifyAction::action(Stream& stream) const
{
	auto& format_settings = const_cast<Stream::FormatSettings&>(stream.format_settings());
	format_settings.justify = _justify;
	stream.set_format_settings(format_settings);
}

void IntWidthAction::action(Stream& stream) const
{
	auto& format_settings = const_cast<Stream::FormatSettings&>(stream.format_settings());

	format_settings.int_width = _width;
	format_settings.padding_char = _pad;

	stream.set_format_settings(format_settings);
}

void ReadSizeAction::action(Stream& stream) const
{
    Stream::ReadSize& read_size = const_cast<Stream::ReadSize&>(stream.read_size());
    read_size.data_size = _read_size;
    stream.set_read_size(read_size);
}