#pragma once

#include <vector>
#include <stdint.h>
#include <algorithm>

template <class T>
class RotateBuffer
{
private:
    std::vector<T> mData;
	typename std::vector<T>::size_type mMaxSize;

public:

    RotateBuffer() : mData(), mMaxSize(0) {}

	void alloc(typename std::vector<T>::size_type size)
	{
		mMaxSize = size;
		mData.reserve(mMaxSize);
	}

	void clear()
	{
		mData.clear();
	}

	uint16_t getSize() const { return mData.capacity(); }

    void put(T item)
	{
		if (mData.size() == mMaxSize) {
            std::rotate(mData.begin(), mData.begin() + 1, mData.end());
            mData.pop_back();
		}

		mData.push_back(item);
	}

	const std::vector<T>& vector() const { return mData; };

	uint8_t* buffer()
    {
        return reinterpret_cast<uint8_t*>(mData.data());
    }

	uint16_t bufferSize()
	{
		return (mData.capacity() * sizeof(T));
	}

	T median()
	{
		size_t size = mData.size();

		if (size == 0) {
			return T();  // Undefined, really.
		} else {
			std::sort(mData.begin(), mData.end());
			if (size % 2 == 0) {
				return (mData[size / 2 - 1] + mData[size / 2]) / 2;
			} else {
				return mData[size / 2];
			}
		}
	}
};