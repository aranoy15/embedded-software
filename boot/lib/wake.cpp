#include <cstring>
#include <wake.hpp>

using namespace lib::wake;

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
	return false;
}
