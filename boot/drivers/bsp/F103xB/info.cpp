#include <drivers/bsp/F103xB/info.hpp>
#include <main.h>

namespace bsp
{
bool AppArea::full_erase()
{
    FLASH_EraseInitTypeDef erase_inits;
	uint32_t errors;

	uint32_t start = get_page(app_address);
	uint32_t count = (get_page(get_end_address()) - start) / page_size;

	erase_inits.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_inits.PageAddress = start;
	erase_inits.NbPages = count;

	return (HAL_FLASHEx_Erase(&erase_inits, &errors) == HAL_OK);
}

bool AppArea::write(uint32_t addr, const uint8_t* data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i += sizeof(uint32_t)) {
		uint32_t address = addr + i;
		uint32_t word_to_write = *(uint32_t*)(data + i);

		auto status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, word_to_write);

		if (status != HAL_OK) return false;
	}

	return true;
}

uint32_t AppArea::get_page(uint32_t address)
{
    uint32_t sector = 0;

	if ((address < ADDR_FLASH_PAGE_1) and (address >= ADDR_FLASH_PAGE_0)) {
		sector = ADDR_FLASH_PAGE_0;
	} else if ((address < ADDR_FLASH_PAGE_2) and
	           (address >= ADDR_FLASH_PAGE_1)) {
		sector = ADDR_FLASH_PAGE_1;
	} else if ((address < ADDR_FLASH_PAGE_3) and
	           (address >= ADDR_FLASH_PAGE_2)) {
		sector = ADDR_FLASH_PAGE_2;
	} else if ((address < ADDR_FLASH_PAGE_4) and
	           (address >= ADDR_FLASH_PAGE_3)) {
		sector = ADDR_FLASH_PAGE_3;
	} else if ((address < ADDR_FLASH_PAGE_5) and
	           (address >= ADDR_FLASH_PAGE_4)) {
		sector = ADDR_FLASH_PAGE_4;
	} else if ((address < ADDR_FLASH_PAGE_6) and
	           (address >= ADDR_FLASH_PAGE_5)) {
		sector = ADDR_FLASH_PAGE_5;
	} else if ((address < ADDR_FLASH_PAGE_7) and
	           (address >= ADDR_FLASH_PAGE_6)) {
		sector = ADDR_FLASH_PAGE_6;
	} else if ((address < ADDR_FLASH_PAGE_8) and
	           (address >= ADDR_FLASH_PAGE_7)) {
		sector = ADDR_FLASH_PAGE_7;
	} else if ((address < ADDR_FLASH_PAGE_9) and
	           (address >= ADDR_FLASH_PAGE_8)) {
		sector = ADDR_FLASH_PAGE_8;
	} else if ((address < ADDR_FLASH_PAGE_10) and
	           (address >= ADDR_FLASH_PAGE_9)) {
		sector = ADDR_FLASH_PAGE_9;
	} else if ((address < ADDR_FLASH_PAGE_11) and
	           (address >= ADDR_FLASH_PAGE_10)) {
		sector = ADDR_FLASH_PAGE_10;
	} else if ((address < ADDR_FLASH_PAGE_12) and
	           (address >= ADDR_FLASH_PAGE_11)) {
		sector = ADDR_FLASH_PAGE_11;
	} else if ((address < ADDR_FLASH_PAGE_13) and
	           (address >= ADDR_FLASH_PAGE_12)) {
		sector = ADDR_FLASH_PAGE_12;
	} else if ((address < ADDR_FLASH_PAGE_14) and
	           (address >= ADDR_FLASH_PAGE_13)) {
		sector = ADDR_FLASH_PAGE_13;
	} else if ((address < ADDR_FLASH_PAGE_15) and
	           (address >= ADDR_FLASH_PAGE_14)) {
		sector = ADDR_FLASH_PAGE_14;
	} else if ((address < ADDR_FLASH_PAGE_16) and
	           (address >= ADDR_FLASH_PAGE_15)) {
		sector = ADDR_FLASH_PAGE_15;
	} else if ((address < ADDR_FLASH_PAGE_17) and
	           (address >= ADDR_FLASH_PAGE_16)) {
		sector = ADDR_FLASH_PAGE_16;
	} else if ((address < ADDR_FLASH_PAGE_18) and
	           (address >= ADDR_FLASH_PAGE_17)) {
		sector = ADDR_FLASH_PAGE_17;
	} else if ((address < ADDR_FLASH_PAGE_19) and
	           (address >= ADDR_FLASH_PAGE_18)) {
		sector = ADDR_FLASH_PAGE_18;
	} else if ((address < ADDR_FLASH_PAGE_20) and
	           (address >= ADDR_FLASH_PAGE_19)) {
		sector = ADDR_FLASH_PAGE_19;
	} else if ((address < ADDR_FLASH_PAGE_21) and
	           (address >= ADDR_FLASH_PAGE_20)) {
		sector = ADDR_FLASH_PAGE_20;
	} else if ((address < ADDR_FLASH_PAGE_22) and
	           (address >= ADDR_FLASH_PAGE_21)) {
		sector = ADDR_FLASH_PAGE_21;
	} else if ((address < ADDR_FLASH_PAGE_23) and
	           (address >= ADDR_FLASH_PAGE_22)) {
		sector = ADDR_FLASH_PAGE_22;
	} else if ((address < ADDR_FLASH_PAGE_24) and
	           (address >= ADDR_FLASH_PAGE_23)) {
		sector = ADDR_FLASH_PAGE_23;
	} else if ((address < ADDR_FLASH_PAGE_25) and
	           (address >= ADDR_FLASH_PAGE_24)) {
		sector = ADDR_FLASH_PAGE_24;
	} else if ((address < ADDR_FLASH_PAGE_26) and
	           (address >= ADDR_FLASH_PAGE_25)) {
		sector = ADDR_FLASH_PAGE_25;
	} else if ((address < ADDR_FLASH_PAGE_27) and
	           (address >= ADDR_FLASH_PAGE_26)) {
		sector = ADDR_FLASH_PAGE_26;
	} else if ((address < ADDR_FLASH_PAGE_28) and
	           (address >= ADDR_FLASH_PAGE_27)) {
		sector = ADDR_FLASH_PAGE_27;
	} else if ((address < ADDR_FLASH_PAGE_29) and
	           (address >= ADDR_FLASH_PAGE_28)) {
		sector = ADDR_FLASH_PAGE_28;
	} else if ((address < ADDR_FLASH_PAGE_30) and
	           (address >= ADDR_FLASH_PAGE_29)) {
		sector = ADDR_FLASH_PAGE_29;
	} else if ((address < ADDR_FLASH_PAGE_31) and
	           (address >= ADDR_FLASH_PAGE_30)) {
		sector = ADDR_FLASH_PAGE_30;
	} else if ((address < ADDR_FLASH_PAGE_32) and
	           (address >= ADDR_FLASH_PAGE_31)) {
		sector = ADDR_FLASH_PAGE_31;
	} else if ((address < ADDR_FLASH_PAGE_33) and
	           (address >= ADDR_FLASH_PAGE_32)) {
		sector = ADDR_FLASH_PAGE_32;
	} else if ((address < ADDR_FLASH_PAGE_34) and
	           (address >= ADDR_FLASH_PAGE_33)) {
		sector = ADDR_FLASH_PAGE_33;
	} else if ((address < ADDR_FLASH_PAGE_35) and
	           (address >= ADDR_FLASH_PAGE_34)) {
		sector = ADDR_FLASH_PAGE_34;
	} else if ((address < ADDR_FLASH_PAGE_36) and
	           (address >= ADDR_FLASH_PAGE_35)) {
		sector = ADDR_FLASH_PAGE_35;
	} else if ((address < ADDR_FLASH_PAGE_37) and
	           (address >= ADDR_FLASH_PAGE_36)) {
		sector = ADDR_FLASH_PAGE_36;
	} else if ((address < ADDR_FLASH_PAGE_38) and
	           (address >= ADDR_FLASH_PAGE_37)) {
		sector = ADDR_FLASH_PAGE_37;
	} else if ((address < ADDR_FLASH_PAGE_39) and
	           (address >= ADDR_FLASH_PAGE_38)) {
		sector = ADDR_FLASH_PAGE_38;
	} else if ((address < ADDR_FLASH_PAGE_40) and
	           (address >= ADDR_FLASH_PAGE_39)) {
		sector = ADDR_FLASH_PAGE_39;
	} else if ((address < ADDR_FLASH_PAGE_41) and
	           (address >= ADDR_FLASH_PAGE_40)) {
		sector = ADDR_FLASH_PAGE_40;
	} else if ((address < ADDR_FLASH_PAGE_42) and
	           (address >= ADDR_FLASH_PAGE_41)) {
		sector = ADDR_FLASH_PAGE_41;
	} else if ((address < ADDR_FLASH_PAGE_43) and
	           (address >= ADDR_FLASH_PAGE_42)) {
		sector = ADDR_FLASH_PAGE_42;
	} else if ((address < ADDR_FLASH_PAGE_44) and
	           (address >= ADDR_FLASH_PAGE_43)) {
		sector = ADDR_FLASH_PAGE_43;
	} else if ((address < ADDR_FLASH_PAGE_45) and
	           (address >= ADDR_FLASH_PAGE_44)) {
		sector = ADDR_FLASH_PAGE_44;
	} else if ((address < ADDR_FLASH_PAGE_46) and
	           (address >= ADDR_FLASH_PAGE_45)) {
		sector = ADDR_FLASH_PAGE_45;
	} else if ((address < ADDR_FLASH_PAGE_47) and
	           (address >= ADDR_FLASH_PAGE_46)) {
		sector = ADDR_FLASH_PAGE_46;
	} else if ((address < ADDR_FLASH_PAGE_48) and
	           (address >= ADDR_FLASH_PAGE_47)) {
		sector = ADDR_FLASH_PAGE_47;
	} else if ((address < ADDR_FLASH_PAGE_49) and
	           (address >= ADDR_FLASH_PAGE_48)) {
		sector = ADDR_FLASH_PAGE_48;
	} else if ((address < ADDR_FLASH_PAGE_50) and
	           (address >= ADDR_FLASH_PAGE_49)) {
		sector = ADDR_FLASH_PAGE_49;
	} else if ((address < ADDR_FLASH_PAGE_51) and
	           (address >= ADDR_FLASH_PAGE_50)) {
		sector = ADDR_FLASH_PAGE_50;
	} else if ((address < ADDR_FLASH_PAGE_52) and
	           (address >= ADDR_FLASH_PAGE_51)) {
		sector = ADDR_FLASH_PAGE_51;
	} else if ((address < ADDR_FLASH_PAGE_53) and
	           (address >= ADDR_FLASH_PAGE_52)) {
		sector = ADDR_FLASH_PAGE_52;
	} else if ((address < ADDR_FLASH_PAGE_54) and
	           (address >= ADDR_FLASH_PAGE_53)) {
		sector = ADDR_FLASH_PAGE_53;
	} else if ((address < ADDR_FLASH_PAGE_55) and
	           (address >= ADDR_FLASH_PAGE_54)) {
		sector = ADDR_FLASH_PAGE_54;
	} else if ((address < ADDR_FLASH_PAGE_56) and
	           (address >= ADDR_FLASH_PAGE_55)) {
		sector = ADDR_FLASH_PAGE_55;
	} else if ((address < ADDR_FLASH_PAGE_57) and
	           (address >= ADDR_FLASH_PAGE_56)) {
		sector = ADDR_FLASH_PAGE_56;
	} else if ((address < ADDR_FLASH_PAGE_58) and
	           (address >= ADDR_FLASH_PAGE_57)) {
		sector = ADDR_FLASH_PAGE_57;
	} else if ((address < ADDR_FLASH_PAGE_59) and
	           (address >= ADDR_FLASH_PAGE_58)) {
		sector = ADDR_FLASH_PAGE_58;
	} else if ((address < ADDR_FLASH_PAGE_60) and
	           (address >= ADDR_FLASH_PAGE_59)) {
		sector = ADDR_FLASH_PAGE_59;
	} else if ((address < ADDR_FLASH_PAGE_61) and
	           (address >= ADDR_FLASH_PAGE_60)) {
		sector = ADDR_FLASH_PAGE_60;
	} else if ((address < ADDR_FLASH_PAGE_62) and
	           (address >= ADDR_FLASH_PAGE_61)) {
		sector = ADDR_FLASH_PAGE_61;
	} else if ((address < ADDR_FLASH_PAGE_63) and
	           (address >= ADDR_FLASH_PAGE_62)) {
		sector = ADDR_FLASH_PAGE_62;
	} else if ((address < ADDR_FLASH_PAGE_64) and
	           (address >= ADDR_FLASH_PAGE_63)) {
		sector = ADDR_FLASH_PAGE_63;
	} else if ((address < ADDR_FLASH_PAGE_65) and
	           (address >= ADDR_FLASH_PAGE_64)) {
		sector = ADDR_FLASH_PAGE_64;
	} else if ((address < ADDR_FLASH_PAGE_66) and
	           (address >= ADDR_FLASH_PAGE_65)) {
		sector = ADDR_FLASH_PAGE_65;
	} else if ((address < ADDR_FLASH_PAGE_67) and
	           (address >= ADDR_FLASH_PAGE_66)) {
		sector = ADDR_FLASH_PAGE_66;
	} else if ((address < ADDR_FLASH_PAGE_68) and
	           (address >= ADDR_FLASH_PAGE_67)) {
		sector = ADDR_FLASH_PAGE_67;
	} else if ((address < ADDR_FLASH_PAGE_69) and
	           (address >= ADDR_FLASH_PAGE_68)) {
		sector = ADDR_FLASH_PAGE_68;
	} else if ((address < ADDR_FLASH_PAGE_70) and
	           (address >= ADDR_FLASH_PAGE_69)) {
		sector = ADDR_FLASH_PAGE_69;
	} else if ((address < ADDR_FLASH_PAGE_71) and
	           (address >= ADDR_FLASH_PAGE_70)) {
		sector = ADDR_FLASH_PAGE_70;
	} else if ((address < ADDR_FLASH_PAGE_72) and
	           (address >= ADDR_FLASH_PAGE_71)) {
		sector = ADDR_FLASH_PAGE_71;
	} else if ((address < ADDR_FLASH_PAGE_73) and
	           (address >= ADDR_FLASH_PAGE_72)) {
		sector = ADDR_FLASH_PAGE_72;
	} else if ((address < ADDR_FLASH_PAGE_74) and
	           (address >= ADDR_FLASH_PAGE_73)) {
		sector = ADDR_FLASH_PAGE_73;
	} else if ((address < ADDR_FLASH_PAGE_75) and
	           (address >= ADDR_FLASH_PAGE_74)) {
		sector = ADDR_FLASH_PAGE_74;
	} else if ((address < ADDR_FLASH_PAGE_76) and
	           (address >= ADDR_FLASH_PAGE_75)) {
		sector = ADDR_FLASH_PAGE_75;
	} else if ((address < ADDR_FLASH_PAGE_77) and
	           (address >= ADDR_FLASH_PAGE_76)) {
		sector = ADDR_FLASH_PAGE_76;
	} else if ((address < ADDR_FLASH_PAGE_78) and
	           (address >= ADDR_FLASH_PAGE_77)) {
		sector = ADDR_FLASH_PAGE_77;
	} else if ((address < ADDR_FLASH_PAGE_79) and
	           (address >= ADDR_FLASH_PAGE_78)) {
		sector = ADDR_FLASH_PAGE_78;
	} else if ((address < ADDR_FLASH_PAGE_80) and
	           (address >= ADDR_FLASH_PAGE_79)) {
		sector = ADDR_FLASH_PAGE_79;
	} else if ((address < ADDR_FLASH_PAGE_81) and
	           (address >= ADDR_FLASH_PAGE_80)) {
		sector = ADDR_FLASH_PAGE_80;
	} else if ((address < ADDR_FLASH_PAGE_82) and
	           (address >= ADDR_FLASH_PAGE_81)) {
		sector = ADDR_FLASH_PAGE_81;
	} else if ((address < ADDR_FLASH_PAGE_83) and
	           (address >= ADDR_FLASH_PAGE_82)) {
		sector = ADDR_FLASH_PAGE_82;
	} else if ((address < ADDR_FLASH_PAGE_84) and
	           (address >= ADDR_FLASH_PAGE_83)) {
		sector = ADDR_FLASH_PAGE_83;
	} else if ((address < ADDR_FLASH_PAGE_85) and
	           (address >= ADDR_FLASH_PAGE_84)) {
		sector = ADDR_FLASH_PAGE_84;
	} else if ((address < ADDR_FLASH_PAGE_86) and
	           (address >= ADDR_FLASH_PAGE_85)) {
		sector = ADDR_FLASH_PAGE_85;
	} else if ((address < ADDR_FLASH_PAGE_87) and
	           (address >= ADDR_FLASH_PAGE_86)) {
		sector = ADDR_FLASH_PAGE_86;
	} else if ((address < ADDR_FLASH_PAGE_88) and
	           (address >= ADDR_FLASH_PAGE_87)) {
		sector = ADDR_FLASH_PAGE_87;
	} else if ((address < ADDR_FLASH_PAGE_89) and
	           (address >= ADDR_FLASH_PAGE_88)) {
		sector = ADDR_FLASH_PAGE_88;
	} else if ((address < ADDR_FLASH_PAGE_90) and
	           (address >= ADDR_FLASH_PAGE_89)) {
		sector = ADDR_FLASH_PAGE_89;
	} else if ((address < ADDR_FLASH_PAGE_91) and
	           (address >= ADDR_FLASH_PAGE_90)) {
		sector = ADDR_FLASH_PAGE_90;
	} else if ((address < ADDR_FLASH_PAGE_92) and
	           (address >= ADDR_FLASH_PAGE_91)) {
		sector = ADDR_FLASH_PAGE_91;
	} else if ((address < ADDR_FLASH_PAGE_93) and
	           (address >= ADDR_FLASH_PAGE_92)) {
		sector = ADDR_FLASH_PAGE_92;
	} else if ((address < ADDR_FLASH_PAGE_94) and
	           (address >= ADDR_FLASH_PAGE_93)) {
		sector = ADDR_FLASH_PAGE_93;
	} else if ((address < ADDR_FLASH_PAGE_95) and
	           (address >= ADDR_FLASH_PAGE_94)) {
		sector = ADDR_FLASH_PAGE_94;
	} else if ((address < ADDR_FLASH_PAGE_96) and
	           (address >= ADDR_FLASH_PAGE_95)) {
		sector = ADDR_FLASH_PAGE_95;
	} else if ((address < ADDR_FLASH_PAGE_97) and
	           (address >= ADDR_FLASH_PAGE_96)) {
		sector = ADDR_FLASH_PAGE_96;
	} else if ((address < ADDR_FLASH_PAGE_98) and
	           (address >= ADDR_FLASH_PAGE_97)) {
		sector = ADDR_FLASH_PAGE_97;
	} else if ((address < ADDR_FLASH_PAGE_99) and
	           (address >= ADDR_FLASH_PAGE_98)) {
		sector = ADDR_FLASH_PAGE_98;
	} else if ((address < ADDR_FLASH_PAGE_100) and
	           (address >= ADDR_FLASH_PAGE_99)) {
		sector = ADDR_FLASH_PAGE_99;
	} else if ((address < ADDR_FLASH_PAGE_101) and
	           (address >= ADDR_FLASH_PAGE_100)) {
		sector = ADDR_FLASH_PAGE_100;
	} else if ((address < ADDR_FLASH_PAGE_102) and
	           (address >= ADDR_FLASH_PAGE_101)) {
		sector = ADDR_FLASH_PAGE_101;
	} else if ((address < ADDR_FLASH_PAGE_103) and
	           (address >= ADDR_FLASH_PAGE_102)) {
		sector = ADDR_FLASH_PAGE_102;
	} else if ((address < ADDR_FLASH_PAGE_104) and
	           (address >= ADDR_FLASH_PAGE_103)) {
		sector = ADDR_FLASH_PAGE_103;
	} else if ((address < ADDR_FLASH_PAGE_105) and
	           (address >= ADDR_FLASH_PAGE_104)) {
		sector = ADDR_FLASH_PAGE_104;
	} else if ((address < ADDR_FLASH_PAGE_106) and
	           (address >= ADDR_FLASH_PAGE_105)) {
		sector = ADDR_FLASH_PAGE_105;
	} else if ((address < ADDR_FLASH_PAGE_107) and
	           (address >= ADDR_FLASH_PAGE_106)) {
		sector = ADDR_FLASH_PAGE_106;
	} else if ((address < ADDR_FLASH_PAGE_108) and
	           (address >= ADDR_FLASH_PAGE_107)) {
		sector = ADDR_FLASH_PAGE_107;
	} else if ((address < ADDR_FLASH_PAGE_109) and
	           (address >= ADDR_FLASH_PAGE_108)) {
		sector = ADDR_FLASH_PAGE_108;
	} else if ((address < ADDR_FLASH_PAGE_110) and
	           (address >= ADDR_FLASH_PAGE_109)) {
		sector = ADDR_FLASH_PAGE_109;
	} else if ((address < ADDR_FLASH_PAGE_111) and
	           (address >= ADDR_FLASH_PAGE_110)) {
		sector = ADDR_FLASH_PAGE_110;
	} else if ((address < ADDR_FLASH_PAGE_112) and
	           (address >= ADDR_FLASH_PAGE_111)) {
		sector = ADDR_FLASH_PAGE_111;
	} else if ((address < ADDR_FLASH_PAGE_113) and
	           (address >= ADDR_FLASH_PAGE_112)) {
		sector = ADDR_FLASH_PAGE_112;
	} else if ((address < ADDR_FLASH_PAGE_114) and
	           (address >= ADDR_FLASH_PAGE_113)) {
		sector = ADDR_FLASH_PAGE_113;
	} else if ((address < ADDR_FLASH_PAGE_115) and
	           (address >= ADDR_FLASH_PAGE_114)) {
		sector = ADDR_FLASH_PAGE_114;
	} else if ((address < ADDR_FLASH_PAGE_116) and
	           (address >= ADDR_FLASH_PAGE_115)) {
		sector = ADDR_FLASH_PAGE_115;
	} else if ((address < ADDR_FLASH_PAGE_117) and
	           (address >= ADDR_FLASH_PAGE_116)) {
		sector = ADDR_FLASH_PAGE_116;
	} else if ((address < ADDR_FLASH_PAGE_118) and
	           (address >= ADDR_FLASH_PAGE_117)) {
		sector = ADDR_FLASH_PAGE_117;
	} else if ((address < ADDR_FLASH_PAGE_119) and
	           (address >= ADDR_FLASH_PAGE_118)) {
		sector = ADDR_FLASH_PAGE_118;
	} else if ((address < ADDR_FLASH_PAGE_120) and
	           (address >= ADDR_FLASH_PAGE_119)) {
		sector = ADDR_FLASH_PAGE_119;
	} else if ((address < ADDR_FLASH_PAGE_121) and
	           (address >= ADDR_FLASH_PAGE_120)) {
		sector = ADDR_FLASH_PAGE_120;
	} else if ((address < ADDR_FLASH_PAGE_122) and
	           (address >= ADDR_FLASH_PAGE_121)) {
		sector = ADDR_FLASH_PAGE_121;
	} else if ((address < ADDR_FLASH_PAGE_123) and
	           (address >= ADDR_FLASH_PAGE_122)) {
		sector = ADDR_FLASH_PAGE_122;
	} else if ((address < ADDR_FLASH_PAGE_124) and
	           (address >= ADDR_FLASH_PAGE_123)) {
		sector = ADDR_FLASH_PAGE_123;
	} else if ((address < ADDR_FLASH_PAGE_125) and
	           (address >= ADDR_FLASH_PAGE_124)) {
		sector = ADDR_FLASH_PAGE_124;
	} else if ((address < ADDR_FLASH_PAGE_126) and
	           (address >= ADDR_FLASH_PAGE_125)) {
		sector = ADDR_FLASH_PAGE_125;
	} else if ((address < ADDR_FLASH_PAGE_127) and
	           (address >= ADDR_FLASH_PAGE_126)) {
		sector = ADDR_FLASH_PAGE_126;
	} else {
		sector = ADDR_FLASH_PAGE_127;
	}


	return sector;
}

uint32_t AppArea::get_end_address()
{
    return ADDR_FLASH_PAGE_127 + page_size - 1;
}

void AppArea::lock()
{
    HAL_FLASH_Lock();
}
void AppArea::unlock()
{
    HAL_FLASH_Unlock();
}

void AppArea::jump_to_app()
{
    //__disable_irq(); // запрещаем прерывания

	SCB->VTOR = app_address;
	uint32_t *int_vector_table = (uint32_t *)app_address;

	typedef void (*ResetVectorFunc)();
	ResetVectorFunc reset_vector = (ResetVectorFunc)int_vector_table[1];


	reset_vector();
}
}
