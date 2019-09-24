#include <bootinfo.h>
#include <bsp.h>

void bootAction();

int main(void)
{
	bsp::init();

	bootAction();

	__disable_irq(); // запрещаем прерывания

	SCB->VTOR = BootInfo::appStartAddress;
	uint32_t *int_vector_table = (uint32_t *)BootInfo::appStartAddress;

	typedef void (*ResetVectorFunc)();
	ResetVectorFunc resetVector = (ResetVectorFunc)int_vector_table[1];


	resetVector();
}

extern "C" {

void SysTick_Handler(void)
{
	HAL_IncTick();
}
}