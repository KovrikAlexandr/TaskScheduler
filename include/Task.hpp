#pragma once

#include "declarations.hpp"
#include "Function.hpp"
#include "Any.hpp"
#include "FutureResult.hpp"



template <typename T>
class Dependancy {
public:

    typedef T stored_type;
    typedef T result_type;

    Dependancy(T value) 
        : _value(value)
    {}

    T result() {
        return _value;
    }

private:
    T _value;
};

template <typename R, typename T, typename U>
class Dependancy < FutureResult<R, Task<T, U>> > {
public:

    typedef FutureResult<R, Task<T, U>> stored_type;
    typedef typename stored_type::result_type result_type;

    Dependancy(stored_type value)
        : _value(value)
    {}

    result_type result() {
        return _value.result();
    }

private:
    stored_type _value;
};


class BaseTask {
public:
    virtual void Compute() = 0;
    virtual ~BaseTask() = default;
};

template <typename Arg1, typename Arg2>
class Task : public BaseTask {
public:

    typedef my_std::function<my_std::any(typename Dependancy<Arg1>::result_type, typename Dependancy<Arg2>::result_type)> _Callable;
    
    Task(_Callable callable, Dependancy<Arg1> first_argument, Dependancy<Arg2> second_argument) 
       :    _callable(callable),
            _result(),
            _first_argument(first_argument),
            _second_argument(second_argument)
    {}

    my_std::any result() {
        Compute();
        return _result;
    }

private:
    _Callable _callable;
    my_std::any _result;

    Dependancy<Arg1> _first_argument;
    Dependancy<Arg2> _second_argument;

    void Compute() override {
        if (!_result.has_value()) {
            _result = _callable(_first_argument.result(), _second_argument.result());
        }
    }
};

template <typename Arg1>
class Task<Arg1, void> : public BaseTask {
public:

    typedef my_std::function<my_std::any(typename Dependancy<Arg1>::result_type)> _Callable;
    
    Task(_Callable callable, Dependancy<Arg1> first_argument) 
       :    _callable(callable),
            _result(),
            _first_argument(first_argument)
    {}

    my_std::any result() {
        Compute();
        return _result;
    }

private:
    _Callable _callable;
    my_std::any _result;
    Dependancy<Arg1> _first_argument;

    void Compute() override {
        if (!_result.has_value()) {
            _result = _callable(_first_argument.result());
        }
    }
};

template <>
class Task<void, void> : public BaseTask {
public:

    typedef my_std::function<my_std::any()> _Callable;
    
    Task(_Callable callable)
       :    _callable(callable),
            _result()
    {}

    my_std::any result() {
        Compute();
        return _result;
    }

private:
    _Callable _callable;
    my_std::any _result;

    void Compute() override {
        if (!_result.has_value()) {
            _result = _callable();
        }
    }
};
