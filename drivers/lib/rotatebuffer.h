#ifndef DRIVERS_LIB_ROTATEBUFFER
#define DRIVERS_LIB_ROTATEBUFFER

#include <vector>
#include <cinttypes>
#include <algorithm>
#include <numeric>

#include <lib/stream/stream.hpp>

namespace lib
{

template <class T>
class RotateBuffer
{

public:
    RotateBuffer() : _data(), _max_size(0) {}

	void alloc(typename std::vector<T>::size_type size)
	{
		_max_size = size;
		_data.reserve(_max_size);
	}

	bool is_full() const
	{
		return _data.size() == _max_size;
	}

	void clear()
	{
		_data.clear();
	}

	uint16_t capacity() const { return _data.capacity(); }

    void put(T item)
	{
		if (is_full()) {
            std::rotate(_data.begin(), _data.begin() + 1, _data.end());
            _data.pop_back();
		}

		_data.push_back(item);
	}

	T last() 
	{ 
		if (_data.empty()) return T();
		return _data.back() - 1;
	}

	const std::vector<T>& vector() const { return _data; };

	uint8_t* buffer()
    {
        return reinterpret_cast<uint8_t*>(_data.data());
    }

	uint16_t buffer_size()
	{
		return (_data.capacity() * sizeof(T));
	}

	T median()
	{
		size_t size = _data.size();

		if (size == 0) {
			return T();  // Undefined, really.
		} else {
			std::sort(_data.begin(), _data.end());
			if (size % 2 == 0) {
				return (_data[size / 2 - 1] + _data[size / 2]) / 2;
			} else {
				return _data[size / 2];
			}
		}
	}

	T average()
	{
		T result = std::accumulate(_data.begin(), _data.end(), T());

		return (result / _data.size());
	}

	T min()
	{
		if (_data.size() == 0) return T();
		auto res = *std::min_element(_data.begin(), _data.end());
		return res;
	}

	T max()
	{
		if (_data.size() == 0) return T();
		auto res = *std::max_element(_data.begin(), _data.end());

		return res;
	}

private:
    std::vector<T> _data;
	typename std::vector<T>::size_type _max_size;
};

template<class T>
stream::Stream& operator<<(stream::Stream& s, RotateBuffer<T> rb)
{
	for (const auto& item : rb.vector()) {
		s << item << ' ';
	}

	return s;
}

}

#endif /* DRIVERS_LIB_ROTATEBUFFER */
