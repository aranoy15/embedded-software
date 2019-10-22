#include <archivetask.h>

namespace
{
__attribute__((constructor)) void reg()
{
	TaskScheduler::instance()->reg(new ArchiveTask());
}
}  // namespace

ArchiveTask::ArchiveTask() : TaskBase(10 * Time::second(), TaskPriority::Low) {}

ArchiveTask::~ArchiveTask() {}

void ArchiveTask::setup()
{

}

void ArchiveTask::func()
{
    FatFsClass& fatFs = *FatFsClass::instance();
    bool cd = fatFs.cardDetected();
	if (cd) {
		if (fatFs.init()) {

			FileFs file;
			std::string fileName = "/TEST.txt";

			uint8_t mode = FA_WRITE | FA_READ;

			if (fatFs.fileExists(fileName.data()))
				mode |= FA_OPEN_APPEND;
			else
				mode |= FA_CREATE_ALWAYS;

			if (file.open(fileName.data(), mode)) {
				file.close();
			}
		}
	}
}