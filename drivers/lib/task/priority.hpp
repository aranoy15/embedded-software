#ifndef DRIVERS_LIB_TASK_PRIORITY
#define DRIVERS_LIB_TASK_PRIORITY

namespace lib::task
{
enum class Priority
{
    Idle          = -3,          ///< priority: idle (lowest)
    Low           = -2,          ///< priority: low
    BelowNormal   = -1,          ///< priority: below normal
    Normal        =  0,          ///< priority: normal (default)
    AboveNormal   = +1,          ///< priority: above normal
    High          = +2,          ///< priority: high
    Realtime      = +3,          ///< priority: realtime (highest)
};
}

#endif /* DRIVERS_LIB_TASK_PRIORITY */
