#include <bsp.hpp>

void boot_action();

int main(void)
{
	bsp::init();

	boot_action();

	bsp::AppArea::jump_to_app();	
}
