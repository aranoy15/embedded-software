#include <cstring>
#include <wake.hpp>

using namespace lib::wake;

uint8_t Protocol::_crc_table[256] = {
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97, 0xB9, 0x88, 0xDB, 0xEA,
    0x7D, 0x4C, 0x1F, 0x2E, 0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
    0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D, 0x86, 0xB7, 0xE4, 0xD5,
    0x42, 0x73, 0x20, 0x11, 0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
    0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52, 0x7C, 0x4D, 0x1E, 0x2F,
    0xB8, 0x89, 0xDA, 0xEB, 0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
    0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13, 0x7E, 0x4F, 0x1C, 0x2D,
    0xBA, 0x8B, 0xD8, 0xE9, 0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C, 0x02, 0x33, 0x60, 0x51,
    0xC6, 0xF7, 0xA4, 0x95, 0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
    0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6, 0x7A, 0x4B, 0x18, 0x29,
    0xBE, 0x8F, 0xDC, 0xED, 0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE, 0x80, 0xB1, 0xE2, 0xD3,
    0x44, 0x75, 0x26, 0x17, 0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
    0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2, 0xBF, 0x8E, 0xDD, 0xEC,
    0x7B, 0x4A, 0x19, 0x28, 0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0, 0xFE, 0xCF, 0x9C, 0xAD,
    0x3A, 0x0B, 0x58, 0x69, 0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
    0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A, 0xC1, 0xF0, 0xA3, 0x92,
    0x05, 0x34, 0x67, 0x56, 0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
    0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15, 0x3B, 0x0A, 0x59, 0x68,
    0xFF, 0xCE, 0x9D, 0xAC};

Packet::Packet() : _address(0), _command(0), _length(0), _data(nullptr)
{
}

Packet::Packet(uint8_t address, uint8_t command)
    : _address(address), _command(command), _length(0), _data(nullptr)
{
	_address = (1 << 7) | _address;
	_command = (command & ~(1 << 7));
}

Packet::~Packet() {}

Packet::Packet(const Packet& packet)
    : _address(packet._address),
      _command(packet._command),
      _length(packet._length),
      _data(packet._data)
{
}

bool Protocol::process(uint8_t b) noexcept
{
    if (_packet_receive) _packet_receive = false;

    if (b == FEND and _packet_started) {
        clear();
    } else if (_packet_started) {
        if (b == FEND) _byte_stuffing = true;

        if (b == TFEND and _byte_stuffing) {
            pop();
            push(FEND);
            _byte_stuffing = false;
        } else if (b == TFESC and _byte_stuffing) {
            pop();
            push(FESC);
            _byte_stuffing = false;
        } else {
            push(b);
        }

        if (_size == header_length) {
            _length = _data[length_index];
            _data_started = true;
        }

        if (_data_started and _size == packet_size()) {
            uint8_t crc_calc = crc8(_data, _size - 1);
            uint8_t crc_actual = _data[_size - 2];

            if (crc_calc == crc_actual) {
                _packet_receive = true;
                return true;
            } else {
                clear();
            }
        }
    } else if (b == FEND) {
        push(b);
        _packet_started = true;
    }

    return false;
}

void Protocol::push(uint8_t b) noexcept
{
    _data[_size++] = b;
}

uint8_t Protocol::pop() noexcept
{
    if (_size <= 0) return 0;
    uint8_t result = _data[_size--];

    return  result;
}

void Protocol::clear() noexcept
{
    _packet_started = false;
    _byte_stuffing = false;
    _data_started = false;
    _packet_receive = false;
    _size = 0;
    _length = 0;
}

uint8_t Protocol::packet_size() noexcept
{
    return header_length + _length + crc_length + (_byte_stuffing ? 1 : 0);
}

Packet Protocol::unpack() noexcept
{
    Packet result;

    if (_packet_receive) {
        result.set_address(_data[address_index]);
        result.set_command(_data[command_index]);
        result.set_length(_data[length_index]);
        result.set_data(&_data[data_index]);
    }

    return result;
}

uint8_t Protocol::crc8(uint8_t data[], std::size_t size)
{
    uint8_t result = 0xFF;

    for (std::size_t i = 0; i < size; ++i) {
        result = _crc_table[result ^ data[i]];
    }

    return  result;
}
