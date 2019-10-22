#pragma once

//#include <datetimemanager.h>
#include <fatfsclass.h>
#include <flash.h>
#include <taskscheduler.h>

class ArchiveTask : public TaskBase
{
public:
    ArchiveTask();
    virtual ~ArchiveTask();

    void setup() override;
    void func() override;
};