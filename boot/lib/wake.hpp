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

    void set_address(uint8_t address) { _address = (1 << 7) | address; }
    void set_command(uint8_t command) { _command = (command & ~(1 << 7)); }
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

    enum
    {
        FEND = 0xC0,
        PESC = 0xDB,
        TFEND = 0xDC,
        TFESC = 0xDD
    };

public:
    Protocol() = delete;

    static bool process(uint8_t b) noexcept;
    static Packet get_packet() noexcept;

private:
    inline static uint8_t _data[max_frame_size];
    inline static uint8_t _size = 0;
};
}

#endif /* BOOT_LIB_WAKE */
