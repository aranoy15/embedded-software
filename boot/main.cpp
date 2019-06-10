#include <bootinfo.h>
#include <system.h>
#include <bootuart.h>

int main(void)
{
    stm32system::init(); 

	BootUart uart;
	uart.init(115200);

	uart.puts_("Start application");

    __disable_irq(); // запрещаем прерывания

	SCB->VTOR = BootInfo::appStartAddress;
	uint32_t *int_vector_table = (uint32_t *)BootInfo::appStartAddress;

	typedef void (*ResetVectorFunc)();
	ResetVectorFunc resetVector = (ResetVectorFunc)int_vector_table[1];
	resetVector();
}