#pragma once

// Dependancy
template <typename T>
class Dependancy;

template <typename R, typename T, typename U>
class Dependancy < FutureResult<R, Task<T, U>> >;


// Task
class BaseTask;

template <typename T, typename U>
class Task;

template <typename T>
class Task<T, void>;

template <>
class Task<void, void>;


// FutureResult
template <typename R, typename T>
struct FutureResult;

template <typename R, typename T, typename U>
struct FutureResult<R, Task<T, U>>;


//TTaskScheduler
class TTaskScheduler;

