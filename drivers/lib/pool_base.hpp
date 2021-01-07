#ifndef DRIVERS_LIB_POOL_BASE
#define DRIVERS_LIB_POOL_BASE

#include <vector>
#include <memory>

namespace lib
{

template<typename T>
class PoolBase
{
public:
    using item_t = T;
    using item_ptr = std::unique_ptr<item_t>;
    using storage_t = std::vector<item_ptr>;

protected:
    storage_t _data;

public:
    PoolBase(const PoolBase&) = delete;
    PoolBase operator=(const PoolBase&) = delete;

    PoolBase();
    virtual ~PoolBase();

    template<typename RegT, typename... Args>
    void reg(Args&&... args) noexcept;
};

template<typename T>
PoolBase<T>::PoolBase() : _data() {}

template<typename T>
PoolBase<T>::~PoolBase() {}

template<typename T>
template<typename RegT, typename... Args>
void PoolBase<T>::reg(Args&&... args) noexcept
{
    static_assert(std::is_base_of<T, RegT>::value, "not base class");
    _data.emplace_back(std::make_unique<RegT>(std::forward<Args>(args)...));
}
}

#endif /* DRIVERS_LIB_POOL_BASE */
