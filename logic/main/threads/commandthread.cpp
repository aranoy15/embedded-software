#include <commandthread.h>
#include <string>

CommandThread::CommandThread() : Thread(osPriorityHigh, 3 * 1024, "cmd") {}

void CommandThread::threadFunc()
{
    std::string message = "";

	for(;;) {
		message = getLog().readln();
		if (not message.empty()) {
			getLog().send("received message: " + message);
		} 

		Time::sleep(Time(1));
	}
}