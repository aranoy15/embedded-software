#include <library/utils.h>

namespace utils
{
quint16 crc16(const QByteArray& data)
{
	quint16 crc = 0;

	for (quint8 item : data) {
		crc ^= item;

		bool bitMoved = (crc & 1 << 15) != 0;
		crc <<= 1;

		if (bitMoved) crc |= 1;
	}

	return crc;
}
}  // namespace utils