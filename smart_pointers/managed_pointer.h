//
// Created by Shiping Yao on 2022/5/19.
//
#ifndef LEARNCPP_MANAGED_POINTER_H
#define LEARNCPP_MANAGED_POINTER_H

#include <memory>


namespace learncpp {


template<class Underlying>
class ManagedPointer {

public:
    ManagedPointer() = default;
    explicit ManagedPointer(Underlying* ptr): underlying_(ptr) {}

    explicit ManagedPointer(const std::unique_ptr<Underlying> &smart_ptr): underlying_(smart_ptr.get()) {}

    ManagedPointer(std::nullptr_t null_ptr) noexcept: underlying_(nullptr) {}

    Underlying &operator*() const {return *underlying_;}

    Underlying* operator->() const {return underlying_;}

    Underlying* Get() const { return underlying_; }

    explicit operator bool() const {return underlying_;}

    ManagedPointer& operator=(std::nullptr_t) {
        underlying_ = nullptr;
        return *this;
    }

    template<class NewType>
    ManagedPointer<NewType> CastManagedPointTo() const {
        static_assert(std::is_convertible_v<Underlying*, NewType*> || std::is_base_of_v<Underlying, NewType>);
        return ManagedPointer<NewType>(reinterpret_cast<NewType* >(underlying_));
    }
private:
    Underlying* underlying_;
};

}
#endif //LEARNCPP_MANAGED_POINTER_H
