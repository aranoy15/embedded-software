#ifndef BOOT_LIB_WAKE
#define BOOT_LIB_WAKE

#include <cinttypes>

namespace lib::wake
{
struct Packet final
{
    Packet();
    Packet(uint8_t address, uint8_t command);
    ~Packet();

    Packet(const Packet& packet);
    Packet operator=(const Packet& packet);

    uint8_t address() const { return _address; }
    uint8_t command() const { return _command; }
    uint8_t length() const { return _length; }
    const uint8_t* data() const { return _data; }

    void set_address(uint8_t addr) { _address = (1 << 7) | addr; }
    void set_command(uint8_t cmd) { _command = (cmd & ~(1 << 7)); }
    void set_length(uint8_t len) { _command = (len & ~(1 << 7)); }
    void set_data(uint8_t* data) { _data = data; }

private:
    uint8_t _address;
    uint8_t _command;
    uint8_t _length;
    uint8_t* _data;
};

class Protocol final
{
public:
    static constexpr uint8_t max_frame_size = 255;
    static constexpr uint8_t header_length = 4;
    static constexpr uint8_t crc_length = 1;

    static constexpr uint8_t address_index = 1;
    static constexpr uint8_t command_index = 2;
    static constexpr uint8_t length_index = 3;
    static constexpr uint8_t data_index = 4;

    enum
    {
        FEND = 0xC0,
        FESC = 0xDB,
        TFEND = 0xDC,
        TFESC = 0xDD
    };

public:
    Protocol() = delete;

    static bool process(uint8_t b) noexcept;
    static Packet unpack() noexcept;
    static void pack(const Packet& packet, uint8_t* data) noexcept;

    static void clear() noexcept;

private:
    static uint8_t crc8(uint8_t data[], std::size_t size);

    static void push(uint8_t b) noexcept;
    static uint8_t pop() noexcept;
    static uint8_t packet_size() noexcept;

private:
    inline static uint8_t _data[max_frame_size];
    inline static uint8_t _size = 0;

    static uint8_t _crc_table[256];

    inline static bool _packet_started = false;
    inline static bool _byte_stuffing = false;
    inline static bool _data_started = false;
    inline static bool _packet_receive = false;
    inline static uint8_t _length = 0;
};
}

#endif /* BOOT_LIB_WAKE */
