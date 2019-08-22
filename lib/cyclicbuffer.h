#ifndef SHARELIB_CIRCLEBUFFER_H
#define SHARELIB_CIRCLEBUFFER_H

#include <exception>
#include <stdint.h>
#include <vector>

template <class T>
class CyclicBuffer
{
public:
	class NoDataEx : public std::exception
	{
	};

private:
	std::vector<T> m_data;
	typename std::vector<T>::size_type m_first;
	typename std::vector<T>::size_type m_maxSize;

public:
	CyclicBuffer() : m_data(), m_first(0), m_maxSize(0) {}

	CyclicBuffer(typename std::vector<T>::size_type size)
	{
		m_data.reserve(m_maxSize);
	}

	void alloc(typename std::vector<T>::size_type size)
	{
		m_maxSize = size;
		m_data.reserve(m_maxSize);
	}

	void clear()
	{
		m_data.clear();
		m_first = 0;
	}

	uint16_t getSize() const { return m_data.capacity(); }
	uint16_t getUsedSize() const { return m_data.size() - m_first; }
	uint16_t getFreeSize() const { return getSize() - getUsedSize(); }
	bool isEmpty() const { return getUsedSize() == 0; }
	bool isFull() const { return getUsedSize() == getSize(); }

	void put(T item)
	{
		if (m_data.size() == m_maxSize) {
			if (m_first == 0) return;

			m_data.erase(m_data.begin(), m_data.begin() + m_first);
			m_first = 0;
		}

		m_data.push_back(item);
	}

	T get()
	{
		//if (isEmpty()) throw NoDataEx();
        return m_data.at(m_first++);
	}

    uint8_t* buffer()
    {
        return reinterpret_cast<uint8_t*>(&m_data[m_first]);
    }

	uint16_t bufferSize()
	{
		return (m_data.capacity() * sizeof(T));
	}
};

#endif /* SHARELIB_CIRCLEBUFFER_H */