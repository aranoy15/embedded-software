//#include <bootinfo.h>
#include <bsp.hpp>

void boot_action();

int main(void)
{
	bsp::init();

	bsp::usart::init();

	boot_action();

	bsp::AppArea::jump_to_app();	
}
