#pragma once

#include <memory>

namespace my_std {

class any {

    struct Base {
        virtual ~Base() = default;
        virtual Base* copy() = 0;
        virtual void* get_value_ptr() = 0;
    };

    template <typename T>
    struct Derived : public Base {

        Derived(const T& t)
            : _t(t)
        {}

        Base* copy() override {
            return new Derived<T>(_t);
        }

        void* get_value_ptr() override {
            return &_t;
        }

        ~Derived() override = default;

        T _t;
    };

public:

    any() 
        : _ptr(nullptr)
    {}

    any(const any& other) 
        : _ptr(other._ptr->copy())
    {}

    any(any&& other) 
        : _ptr(other._ptr)
    {}

    template <typename T>
    any(T value)
        : _ptr(new Derived<T>(std::move(value)))
    {}

    template <typename T>
    any& operator=(T t) {
        delete _ptr;
        _ptr = new Derived<T>(std::move(t));
        return *this;
    }

    any& operator=(const any& other) {
        _ptr = other._ptr->copy();
        return *this;
    }

    any& operator=(any&& other) {
        _ptr = other._ptr;
        other._ptr = nullptr;
        return *this;
    }

    ~any() {
        delete _ptr;
    }

    bool has_value() const {
        return _ptr != nullptr;
    }

    operator bool() const {
        return has_value();
    }

    template <typename T>
    friend T any_cast(any&);

    template <typename T>
    friend T any_cast(any&&);

private:
    Base* _ptr;
};

template <typename T>
T any_cast(any& a) {
    return *static_cast<T*>(a._ptr->get_value_ptr());
}

template <typename T>
T any_cast(any&& a) {
    return *static_cast<T*>(a._ptr->get_value_ptr());
}

}