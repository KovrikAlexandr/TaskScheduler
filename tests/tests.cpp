#include <TaskScheduler.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <cmath>

TEST(AnyTestSuite, EmptyTest) {
    my_std::any a;
}

TEST(AnyTestSuite, ConstructTest) {
    my_std::any a;
    my_std::any b = 10.5f;
    my_std::any c = 25;

    ASSERT_FALSE(a);
    ASSERT_TRUE(b);
}

TEST(AnyTestSuite, AnyCastTest) {
    my_std::any a = 5.5f;
    my_std::any b = 25;

    ASSERT_EQ(my_std::any_cast<float>(a), 5.5f);
    ASSERT_EQ(my_std::any_cast<int>(b), 25);
}

TEST(AnyTestSuite, AssignTest) {
    std::vector<int> v = std::vector<int>({1, 2, 3});
    my_std::any a = v;
    my_std::any b;
    b = a;

    ASSERT_EQ(my_std::any_cast<std::vector<int>>(a), std::vector<int>({1, 2, 3}));;
    ASSERT_EQ(my_std::any_cast<std::vector<int>>(b), std::vector<int>({1, 2, 3}));;
}

TEST (FunctionTestSuite, EmptyTest) {
    my_std::function<void()> f;
    ASSERT_FALSE(f.has_value());
    ASSERT_FALSE(f);
}

TEST (FunctionTestSuite, LambdaTest) {
    int a = 5;
    int b = 15;
    my_std::function<int(int, int)> f = [](int a, int b) { return a + b; };
    ASSERT_EQ(f(a, b), a + b);
}

TEST (FunctionTestSuite, ValueAssignTest) {
    int a = 10;
    int b = 4;

    auto Sum = [](int a, int b) { return a + b; };
    auto Substract = [](int a, int b) { return a - b; };

    my_std::function<int(int, int)> f;

    f = Sum;
    ASSERT_EQ(f(a, b), a + b);

    f = Substract;
    ASSERT_EQ(f(a, b), a - b);
}

TEST (FunctionTestSuite, CopyTest) {
    int a = 10;
    int b = 4;

    auto Sum = [](int a, int b) { return a + b; };
    auto Substract = [](int a, int b) { return a - b; };

    my_std::function<int(int, int)> f = [](int a, int b) { return a + b; };

    auto g = f;
    ASSERT_EQ(g(a, b), a + b);
}

TEST(TTaskSchedulerTestSuite, EmptyTest) {
    TTaskScheduler scheduler;
}

template <typename T>
class C {
public:
    C(T t) = delete;
};

TEST(TTaskSchedulerTestSuite, ZeroArgumentsTasks) {
    TTaskScheduler scheduler;

    auto& id1 = scheduler.add([]() -> float { return std::sqrt(16); });
    auto& id2 = scheduler.add([]() -> int { return 3 + 8; });

    auto result1 = scheduler.getResult<float>(id1);
    auto result2 = scheduler.getResult<int>(id2);
    
    ASSERT_EQ(result1, 4.0);
    ASSERT_EQ(result2, 11);
}

TEST(TTaskSchedulerTestSuite, OneArgumentTasks) {
    TTaskScheduler scheduler;

    double x = 4;
    int y = 3;

    auto& id1 = scheduler.add([](double x) { return std::sqrt(x); }, x);
    auto& id2 = scheduler.add([](int y) { return y * y; }, y);

    auto result1 = scheduler.getResult<double>(id1);
    auto result2 = scheduler.getResult<int>(id2);
    
    ASSERT_EQ(result1, 2.0);
    ASSERT_EQ(result2, 9);
}

TEST(TTaskSchedulerTestSuite, TwoArgumentsTasks) {
    int a = 5;
    int b = 12;
    int c = 2;

    TTaskScheduler scheduler;

    auto& id1 = scheduler.add([](int a, int b) { return a * b; }, a, b);
    auto& id2 = scheduler.add([](int a, int b) { return a - b; }, b, c);

    auto multiply = scheduler.getResult<int>(id1);
    auto substract = scheduler.getResult<int>(id2);

    ASSERT_EQ(multiply, 60);
    ASSERT_EQ(substract, 10);
}

TEST(TTaskSchedulerTestSuite, DependantTasks) {
    float a = 1;
    float b = -2;
    float c = 0;

    TTaskScheduler scheduler;

    auto& id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c  );
    auto& id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));
    auto& id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));
    auto& id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));
    auto& id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));
    auto& id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    auto root1 = scheduler.getResult<float>(id5);
    auto root2 = scheduler.getResult<float>(id6);

    ASSERT_EQ(root1, 2);
    ASSERT_EQ(root2, 0);
}