#pragma once

namespace my_std {

template <typename _Callable>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {

    struct BaseCallable {
        virtual ~BaseCallable() = default;
        virtual R Invoke(Args...) = 0;
        virtual BaseCallable* copy() = 0;
    };

    template <typename T>
    class DerivedCallable : public BaseCallable {
    public:
        DerivedCallable(T callable) 
            : _callable(callable)
        {}

        ~DerivedCallable() override = default;

        R Invoke(Args... args) override {
            return _callable(args...);
        }

        BaseCallable* copy() override {
            return new DerivedCallable<T>(_callable);
        }

    private:
        T _callable;
    };

public:

    typedef R result_type;

    function() 
        : _callable(nullptr)
    {}

    template <typename T>
    function(T callable) 
        : _callable(new DerivedCallable<T>(std::move(callable)))
    {}

    ~function() {
        if (_callable != nullptr) {
            delete _callable;
        }
    }

    function(const function& other) 
        : _callable(other._callable->copy())
    {}

    R operator()(Args... args) {
        return _callable->Invoke(args...);
    }

    function& operator=(const function& other) {
        delete _callable;
        _callable = other._callable->copy();
        return *this;
    }

    template <typename T>
    function& operator=(T callable) {
        delete _callable;
        _callable = new DerivedCallable<T>(std::move(callable));
        return *this;
    }

    operator bool() {
        return has_value();
    }

    bool has_value() {
        return _callable != nullptr;
    }

private:
    BaseCallable* _callable;
};

}