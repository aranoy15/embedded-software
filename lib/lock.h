#ifndef LOCK_H
#define LOCK_H

#include <assert.h>

class Lock;

class Lockable
{
public:
	virtual ~Lockable() {}
	virtual void Lock() {};
	virtual void Unlock() {};
};

class Lock
{
	Lockable& m_lockable;

public:
	Lock(Lockable& lockable) : m_lockable(lockable) { m_lockable.Lock(); }

	~Lock() { m_lockable.Unlock(); }
};

#endif /* LOCK_H */