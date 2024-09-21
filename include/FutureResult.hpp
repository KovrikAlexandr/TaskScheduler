#pragma once

#include "declarations.hpp"

template <typename R, typename T, typename U>
struct FutureResult<R, Task<T, U>> {

    typedef Task<T, U> task_type;
    typedef R result_type;

    FutureResult(task_type& task)
        : _task(task)
    {}

    FutureResult(const FutureResult& other) = default;

    task_type& _task;

    result_type result() {
        return my_std::any_cast<result_type>(_task.result());
    }
};