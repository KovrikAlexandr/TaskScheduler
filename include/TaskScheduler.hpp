#pragma once

#include <vector>

#include "declarations.hpp"
#include "Task.hpp"
#include "FutureResult.hpp"
#include "Any.hpp"
#include "Function.hpp"

class TTaskScheduler {
public:
    TTaskScheduler() = default;

    template <typename _Callable>
    auto& add(_Callable callable) {

        typedef my_std::function<my_std::any()> function_type;
        typedef Task<void, void> task_type;

        auto task = new task_type(function_type(callable));
        _tasks.push_back(task);
        return *task;
    }

    template <typename _Callable, typename Arg1>
    auto& add(_Callable callable, Arg1 first_argument) {

        typedef my_std::function<my_std::any(typename Dependancy<Arg1>::result_type)> function_type;
        typedef Task<Arg1, void> task_type;

        auto task = new task_type(function_type(callable), Dependancy(first_argument));
        _tasks.push_back(task);
        return *task;
    }

    template <typename _Callable, typename Arg1, typename Arg2>
    auto& add(_Callable callable, Arg1 first_argument, Arg2 second_argument) {

        typedef my_std::function< my_std::any(typename Dependancy<Arg1>::result_type, typename Dependancy<Arg2>::result_type)> function_type;
        typedef Task<Arg1, Arg2> task_type;

        auto task = new task_type(function_type(callable), Dependancy(first_argument), Dependancy(second_argument));
        _tasks.push_back(task);
        return *task;
    }

    template <typename R, typename T, typename U>
    R getResult(Task<T, U>& task) {
       return my_std::any_cast<R>(task.result());
    }

    template <typename R, typename T, typename U>
    R getResult(FutureResult<R, Task<T, U>>& fr) {
       return fr.result();
    }

    template <typename R, typename T, typename U>
    auto getFutureResult(Task<T, U>& task) {
        return FutureResult<R, Task<T, U>>(task);
    }

    void executeAll() {
        for (auto task_ptr : _tasks) {
            task_ptr->Compute();
        }
    }

    ~TTaskScheduler() {
        for (auto task_ptr : _tasks) {
            delete task_ptr;
        }
    }

private:
    std::vector<BaseTask*> _tasks;
};
