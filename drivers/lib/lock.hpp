#ifndef LOCK_H
#define LOCK_H

namespace lib
{
class Lock;

class Lockable
{
public:
	virtual ~Lockable() {}
	virtual void lock(){};
	virtual void unlock(){};
};

class Lock
{

public:
	Lock(Lockable& lockable) : _lockable(lockable) { _lockable.lock(); }

	~Lock() { _lockable.unlock(); }

private:
	Lockable& _lockable;
};
}  // namespace lib

#endif /* LOCK_H */