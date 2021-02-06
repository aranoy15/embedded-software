#ifndef DRIVERS_LIB_STREAM_ACTIONBASE
#define DRIVERS_LIB_STREAM_ACTIONBASE

namespace lib::stream
{
class Stream;

class ActionBase
{
    public:
        ActionBase() = default;
        virtual ~ActionBase() {}

        virtual void action(Stream& stream) const = 0;
};
}

#endif /* DRIVERS_LIB_STREAM_ACTIONBASE */
