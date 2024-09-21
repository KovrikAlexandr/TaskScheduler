Данный проект является задание к лабораторной работе универстита ITMO. Поэтому на работу накладывались определённые ограничения.

## Задача

TTaskScheduler является классом, отвечающим за выполнение связанных по данным между собой задач.
Часто, чтобы решить какую либо задачу, требуется выполнить граф вычислений, где узел графа это задача, ребро связь между результатом выполнения одной задачи и параметром для запуска другой.

Пример такой задачи - нахождение корней квадратного уравнения. Решение подобной задачи в лоб - подразумевает вычисление корня из дискриминанта дважды.  Очевидно, что это не оптимально.

Класс использует отложенные вычисления (подсчёт конкретной задачи и всех её зависимостей происходит лишь в момент запроса)

Пример использования:

```cpp
float a = 1;
float b = -2;
float c = 0;

TTaskScheduler scheduler;

auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.getFutureResult<float>(id2));

auto id5 = scheduler.add([]{float a, float v}{return v / (2 * a);}, a, scheduler.getFutureResult<float>(id3));

auto id6 = scheduler.add([]{float a, float v}{return v / (2 * a);},a, scheduler.getFutureResult<float>(id4));

scheduler.executeAll();

std::cout << "x1 = " << scheduler.getResult<int>(id5) << std::endl;
std::cout << "x2 = " << scheduler.getResult<int>(id6) << std::endl;
```

Где getFutureResult это результат выполнения задачи в будущем,

## Интефейс TTaskScheduler

 - **add** - принимает в качестве аргумента задание для него. Возвращает объект описывающий добавленную таску.
 - **getFutureResult<T>** - возвращает объект, из которого в будущем можно получить результат задания, переданного в качестве результата типа Т
 - **getResult<T>** - возвращает результат выполнения задания определенного типа. Вычисляет его если оно еще не подсчитано
 - **executeAll** - выполняет все запланированные задания

## Ограничения к классу

  - Аргументы гарантированно CopyConstructible
  - Возвращаемое значение произвольное, гарантированно CopyConstructible
  - Количество аргументов не больше 2
