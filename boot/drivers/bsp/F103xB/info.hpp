#ifndef BOOT_DRIVERS_BSP_F103XB_INFO
#define BOOT_DRIVERS_BSP_F103XB_INFO

#include <inttypes.h>
#include <cstddef>

namespace bsp
{

struct AppArea final
{
    AppArea() = delete;

    static const uint32_t page_size = 1024; 
    static const uint32_t app_address = APP_START_ADDR;

    static bool full_erase();
	static bool write(uint32_t addr, const uint8_t* data, uint32_t size);

    static void jump_to_app();

	static void lock();
    static void unlock();

private:
    static uint32_t get_page(uint32_t address);	
    static uint32_t get_end_address();

private:
    enum
    {
        ADDR_FLASH_PAGE_0 = 0x08000000, /* Base @ of Page 0, 1 Kbytes */
        ADDR_FLASH_PAGE_1 = 0x08000400, /* Base @ of Page 1, 1 Kbytes */
        ADDR_FLASH_PAGE_2 = 0x08000800, /* Base @ of Page 2, 1 Kbytes */
        ADDR_FLASH_PAGE_3 = 0x08000C00, /* Base @ of Page 3, 1 Kbytes */
        ADDR_FLASH_PAGE_4 = 0x08001000, /* Base @ of Page 4, 1 Kbytes */
        ADDR_FLASH_PAGE_5 = 0x08001400, /* Base @ of Page 5, 1 Kbytes */
        ADDR_FLASH_PAGE_6 = 0x08001800, /* Base @ of Page 6, 1 Kbytes */
        ADDR_FLASH_PAGE_7 = 0x08001C00, /* Base @ of Page 7, 1 Kbytes */
        ADDR_FLASH_PAGE_8 = 0x08002000, /* Base @ of Page 8, 1 Kbytes */
        ADDR_FLASH_PAGE_9 = 0x08002400, /* Base @ of Page 9, 1 Kbytes */
        ADDR_FLASH_PAGE_10 = 0x08002800, /* Base @ of Page 10, 1 Kbytes */
        ADDR_FLASH_PAGE_11 = 0x08002C00, /* Base @ of Page 11, 1 Kbytes */
        ADDR_FLASH_PAGE_12 = 0x08003000, /* Base @ of Page 12, 1 Kbytes */
        ADDR_FLASH_PAGE_13 = 0x08003400, /* Base @ of Page 13, 1 Kbytes */
        ADDR_FLASH_PAGE_14 = 0x08003800, /* Base @ of Page 14, 1 Kbytes */
        ADDR_FLASH_PAGE_15 = 0x08003C00, /* Base @ of Page 15, 1 Kbytes */
        ADDR_FLASH_PAGE_16 = 0x08004000, /* Base @ of Page 16, 1 Kbytes */
        ADDR_FLASH_PAGE_17 = 0x08004400, /* Base @ of Page 17, 1 Kbytes */
        ADDR_FLASH_PAGE_18 = 0x08004800, /* Base @ of Page 18, 1 Kbytes */
        ADDR_FLASH_PAGE_19 = 0x08004C00, /* Base @ of Page 19, 1 Kbytes */
        ADDR_FLASH_PAGE_20 = 0x08005000, /* Base @ of Page 20, 1 Kbytes */
        ADDR_FLASH_PAGE_21 = 0x08005400, /* Base @ of Page 21, 1 Kbytes */
        ADDR_FLASH_PAGE_22 = 0x08005800, /* Base @ of Page 22, 1 Kbytes */
        ADDR_FLASH_PAGE_23 = 0x08005C00, /* Base @ of Page 23, 1 Kbytes */
        ADDR_FLASH_PAGE_24 = 0x08006000, /* Base @ of Page 24, 1 Kbytes */
        ADDR_FLASH_PAGE_25 = 0x08006400, /* Base @ of Page 25, 1 Kbytes */
        ADDR_FLASH_PAGE_26 = 0x08006800, /* Base @ of Page 26, 1 Kbytes */
        ADDR_FLASH_PAGE_27 = 0x08006C00, /* Base @ of Page 27, 1 Kbytes */
        ADDR_FLASH_PAGE_28 = 0x08007000, /* Base @ of Page 28, 1 Kbytes */
        ADDR_FLASH_PAGE_29 = 0x08007400, /* Base @ of Page 29, 1 Kbytes */
        ADDR_FLASH_PAGE_30 = 0x08007800, /* Base @ of Page 30, 1 Kbytes */
        ADDR_FLASH_PAGE_31 = 0x08007C00, /* Base @ of Page 31, 1 Kbytes */
        ADDR_FLASH_PAGE_32 = 0x08008000, /* Base @ of Page 32, 1 Kbytes */
        ADDR_FLASH_PAGE_33 = 0x08008400, /* Base @ of Page 33, 1 Kbytes */
        ADDR_FLASH_PAGE_34 = 0x08008800, /* Base @ of Page 34, 1 Kbytes */
        ADDR_FLASH_PAGE_35 = 0x08008C00, /* Base @ of Page 35, 1 Kbytes */
        ADDR_FLASH_PAGE_36 = 0x08009000, /* Base @ of Page 36, 1 Kbytes */
        ADDR_FLASH_PAGE_37 = 0x08009400, /* Base @ of Page 37, 1 Kbytes */
        ADDR_FLASH_PAGE_38 = 0x08009800, /* Base @ of Page 38, 1 Kbytes */
        ADDR_FLASH_PAGE_39 = 0x08009C00, /* Base @ of Page 39, 1 Kbytes */
        ADDR_FLASH_PAGE_40 = 0x0800A000, /* Base @ of Page 40, 1 Kbytes */
        ADDR_FLASH_PAGE_41 = 0x0800A400, /* Base @ of Page 41, 1 Kbytes */
        ADDR_FLASH_PAGE_42 = 0x0800A800, /* Base @ of Page 42, 1 Kbytes */
        ADDR_FLASH_PAGE_43 = 0x0800AC00, /* Base @ of Page 43, 1 Kbytes */
        ADDR_FLASH_PAGE_44 = 0x0800B000, /* Base @ of Page 44, 1 Kbytes */
        ADDR_FLASH_PAGE_45 = 0x0800B400, /* Base @ of Page 45, 1 Kbytes */
        ADDR_FLASH_PAGE_46 = 0x0800B800, /* Base @ of Page 46, 1 Kbytes */
        ADDR_FLASH_PAGE_47 = 0x0800BC00, /* Base @ of Page 47, 1 Kbytes */
        ADDR_FLASH_PAGE_48 = 0x0800C000, /* Base @ of Page 48, 1 Kbytes */
        ADDR_FLASH_PAGE_49 = 0x0800C400, /* Base @ of Page 49, 1 Kbytes */
        ADDR_FLASH_PAGE_50 = 0x0800C800, /* Base @ of Page 50, 1 Kbytes */
        ADDR_FLASH_PAGE_51 = 0x0800CC00, /* Base @ of Page 51, 1 Kbytes */
        ADDR_FLASH_PAGE_52 = 0x0800D000, /* Base @ of Page 52, 1 Kbytes */
        ADDR_FLASH_PAGE_53 = 0x0800D400, /* Base @ of Page 53, 1 Kbytes */
        ADDR_FLASH_PAGE_54 = 0x0800D800, /* Base @ of Page 54, 1 Kbytes */
        ADDR_FLASH_PAGE_55 = 0x0800DC00, /* Base @ of Page 55, 1 Kbytes */
        ADDR_FLASH_PAGE_56 = 0x0800E000, /* Base @ of Page 56, 1 Kbytes */
        ADDR_FLASH_PAGE_57 = 0x0800E400, /* Base @ of Page 57, 1 Kbytes */
        ADDR_FLASH_PAGE_58 = 0x0800E800, /* Base @ of Page 58, 1 Kbytes */
        ADDR_FLASH_PAGE_59 = 0x0800EC00, /* Base @ of Page 59, 1 Kbytes */
        ADDR_FLASH_PAGE_60 = 0x0800F000, /* Base @ of Page 60, 1 Kbytes */
        ADDR_FLASH_PAGE_61 = 0x0800F400, /* Base @ of Page 61, 1 Kbytes */
        ADDR_FLASH_PAGE_62 = 0x0800F800, /* Base @ of Page 62, 1 Kbytes */
        ADDR_FLASH_PAGE_63 = 0x0800FC00, /* Base @ of Page 63, 1 Kbytes */
        ADDR_FLASH_PAGE_64 = 0x08010000, /* Base @ of Page 64, 1 Kbytes */
        ADDR_FLASH_PAGE_65 = 0x08010400, /* Base @ of Page 65, 1 Kbytes */
        ADDR_FLASH_PAGE_66 = 0x08010800, /* Base @ of Page 66, 1 Kbytes */
        ADDR_FLASH_PAGE_67 = 0x08010C00, /* Base @ of Page 67, 1 Kbytes */
        ADDR_FLASH_PAGE_68 = 0x08011000, /* Base @ of Page 68, 1 Kbytes */
        ADDR_FLASH_PAGE_69 = 0x08011400, /* Base @ of Page 69, 1 Kbytes */
        ADDR_FLASH_PAGE_70 = 0x08011800, /* Base @ of Page 70, 1 Kbytes */
        ADDR_FLASH_PAGE_71 = 0x08011C00, /* Base @ of Page 71, 1 Kbytes */
        ADDR_FLASH_PAGE_72 = 0x08012000, /* Base @ of Page 72, 1 Kbytes */
        ADDR_FLASH_PAGE_73 = 0x08012400, /* Base @ of Page 73, 1 Kbytes */
        ADDR_FLASH_PAGE_74 = 0x08012800, /* Base @ of Page 74, 1 Kbytes */
        ADDR_FLASH_PAGE_75 = 0x08012C00, /* Base @ of Page 75, 1 Kbytes */
        ADDR_FLASH_PAGE_76 = 0x08013000, /* Base @ of Page 76, 1 Kbytes */
        ADDR_FLASH_PAGE_77 = 0x08013400, /* Base @ of Page 77, 1 Kbytes */
        ADDR_FLASH_PAGE_78 = 0x08013800, /* Base @ of Page 78, 1 Kbytes */
        ADDR_FLASH_PAGE_79 = 0x08013C00, /* Base @ of Page 79, 1 Kbytes */
        ADDR_FLASH_PAGE_80 = 0x08014000, /* Base @ of Page 80, 1 Kbytes */
        ADDR_FLASH_PAGE_81 = 0x08014400, /* Base @ of Page 81, 1 Kbytes */
        ADDR_FLASH_PAGE_82 = 0x08014800, /* Base @ of Page 82, 1 Kbytes */
        ADDR_FLASH_PAGE_83 = 0x08014C00, /* Base @ of Page 83, 1 Kbytes */
        ADDR_FLASH_PAGE_84 = 0x08015000, /* Base @ of Page 84, 1 Kbytes */
        ADDR_FLASH_PAGE_85 = 0x08015400, /* Base @ of Page 85, 1 Kbytes */
        ADDR_FLASH_PAGE_86 = 0x08015800, /* Base @ of Page 86, 1 Kbytes */
        ADDR_FLASH_PAGE_87 = 0x08015C00, /* Base @ of Page 87, 1 Kbytes */
        ADDR_FLASH_PAGE_88 = 0x08016000, /* Base @ of Page 88, 1 Kbytes */
        ADDR_FLASH_PAGE_89 = 0x08016400, /* Base @ of Page 89, 1 Kbytes */
        ADDR_FLASH_PAGE_90 = 0x08016800, /* Base @ of Page 90, 1 Kbytes */
        ADDR_FLASH_PAGE_91 = 0x08016C00, /* Base @ of Page 91, 1 Kbytes */
        ADDR_FLASH_PAGE_92 = 0x08017000, /* Base @ of Page 92, 1 Kbytes */
        ADDR_FLASH_PAGE_93 = 0x08017400, /* Base @ of Page 93, 1 Kbytes */
        ADDR_FLASH_PAGE_94 = 0x08017800, /* Base @ of Page 94, 1 Kbytes */
        ADDR_FLASH_PAGE_95 = 0x08017C00, /* Base @ of Page 95, 1 Kbytes */
        ADDR_FLASH_PAGE_96 = 0x08018000, /* Base @ of Page 96, 1 Kbytes */
        ADDR_FLASH_PAGE_97 = 0x08018400, /* Base @ of Page 97, 1 Kbytes */
        ADDR_FLASH_PAGE_98 = 0x08018800, /* Base @ of Page 98, 1 Kbytes */
        ADDR_FLASH_PAGE_99 = 0x08018C00, /* Base @ of Page 99, 1 Kbytes */
        ADDR_FLASH_PAGE_100 = 0x08019000, /* Base @ of Page 100, 1 Kbytes */
        ADDR_FLASH_PAGE_101 = 0x08019400, /* Base @ of Page 101, 1 Kbytes */
        ADDR_FLASH_PAGE_102 = 0x08019800, /* Base @ of Page 102, 1 Kbytes */
        ADDR_FLASH_PAGE_103 = 0x08019C00, /* Base @ of Page 103, 1 Kbytes */
        ADDR_FLASH_PAGE_104 = 0x0801A000, /* Base @ of Page 104, 1 Kbytes */
        ADDR_FLASH_PAGE_105 = 0x0801A400, /* Base @ of Page 105, 1 Kbytes */
        ADDR_FLASH_PAGE_106 = 0x0801A800, /* Base @ of Page 106, 1 Kbytes */
        ADDR_FLASH_PAGE_107 = 0x0801AC00, /* Base @ of Page 107, 1 Kbytes */
        ADDR_FLASH_PAGE_108 = 0x0801B000, /* Base @ of Page 108, 1 Kbytes */
        ADDR_FLASH_PAGE_109 = 0x0801B400, /* Base @ of Page 109, 1 Kbytes */
        ADDR_FLASH_PAGE_110 = 0x0801B800, /* Base @ of Page 110, 1 Kbytes */
        ADDR_FLASH_PAGE_111 = 0x0801BC00, /* Base @ of Page 111, 1 Kbytes */
        ADDR_FLASH_PAGE_112 = 0x0801C000, /* Base @ of Page 112, 1 Kbytes */
        ADDR_FLASH_PAGE_113 = 0x0801C400, /* Base @ of Page 113, 1 Kbytes */
        ADDR_FLASH_PAGE_114 = 0x0801C800, /* Base @ of Page 114, 1 Kbytes */
        ADDR_FLASH_PAGE_115 = 0x0801CC00, /* Base @ of Page 115, 1 Kbytes */
        ADDR_FLASH_PAGE_116 = 0x0801D000, /* Base @ of Page 116, 1 Kbytes */
        ADDR_FLASH_PAGE_117 = 0x0801D400, /* Base @ of Page 117, 1 Kbytes */
        ADDR_FLASH_PAGE_118 = 0x0801D800, /* Base @ of Page 118, 1 Kbytes */
        ADDR_FLASH_PAGE_119 = 0x0801DC00, /* Base @ of Page 119, 1 Kbytes */
        ADDR_FLASH_PAGE_120 = 0x0801E000, /* Base @ of Page 120, 1 Kbytes */
        ADDR_FLASH_PAGE_121 = 0x0801E400, /* Base @ of Page 121, 1 Kbytes */
        ADDR_FLASH_PAGE_122 = 0x0801E800, /* Base @ of Page 122, 1 Kbytes */
        ADDR_FLASH_PAGE_123 = 0x0801EC00, /* Base @ of Page 123, 1 Kbytes */
        ADDR_FLASH_PAGE_124 = 0x0801F000, /* Base @ of Page 124, 1 Kbytes */
        ADDR_FLASH_PAGE_125 = 0x0801F400, /* Base @ of Page 125, 1 Kbytes */
        ADDR_FLASH_PAGE_126 = 0x0801F800, /* Base @ of Page 126, 1 Kbytes */
        ADDR_FLASH_PAGE_127 = 0x0801FC00, /* Base @ of Page 127, 1 Kbytes */
    };
};

}

#endif /* BOOT_DRIVERS_BSP_F103XB_INFO */
