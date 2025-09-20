#include "../includes/ArrayStack.hpp"

#include "TestHarness.hpp"

void test_basic_operations() {
    ArrayStack<int> array_stack;
    CHECK(array_stack.size() == 0);
    CHECK(array_stack.is_empty());

    array_stack.add(0, 10);
    CHECK(array_stack.size() == 1);
    CHECK(!array_stack.is_empty());
    CHECK(array_stack.get(0) == 10);

    array_stack.add(0, 5);
    CHECK(array_stack.size() == 2);
    CHECK(array_stack.get(0) == 5);
    CHECK(array_stack.get(1) == 10);

    array_stack.add(1, 7);
    CHECK(array_stack.size() == 3);
    CHECK(array_stack.get(0) == 5);
    CHECK(array_stack.get(1) == 7);
    CHECK(array_stack.get(2) == 10);

    array_stack.set(1, 8);
    CHECK(array_stack.get(1) == 8);

    int r = array_stack.remove(0);
    CHECK(r == 5);
    CHECK(array_stack.size() == 2);
    CHECK(array_stack.get(0) == 8);
    CHECK(array_stack.get(1) == 10);

    r = array_stack.remove(array_stack.size() - 1);
    CHECK(r == 10);
    CHECK(array_stack.size() == 1);
    CHECK(array_stack.get(0) == 8);

    r = array_stack.remove(0);
    CHECK(r == 8);
    CHECK(array_stack.size() == 0);
    CHECK(array_stack.is_empty());
}

void test_bounds_and_exceptions() {
    ArrayStack<int> array_stack;

    EXPECT_OUT_OF_RANGE(array_stack.get(0));
    EXPECT_OUT_OF_RANGE(array_stack.set(0, 1));
    EXPECT_OUT_OF_RANGE(array_stack.remove(0));
    EXPECT_OUT_OF_RANGE(array_stack.add(-1, 1));
    EXPECT_OUT_OF_RANGE(array_stack.add(1, 1));
}

void test_resize_behavior() {
    ArrayStack<int> array_stack;

    constexpr int kN = 1000;
    for (int i = 0; i < kN; ++i) {
        array_stack.add(i, i);
        CHECK(array_stack.size() == i + 1);
        CHECK(array_stack.get(i) == i);
    }

    for (int i = 0; i < kN - 1; ++i) {
        int val = array_stack.remove(array_stack.size() - 1);
        CHECK(val == kN - 1 - i);
    }
    CHECK(array_stack.size() == 1);
    CHECK(array_stack.get(0) == 0);

    for (int i = 1; i <= kN; ++i) {
        array_stack.add(i, i * 10);
        if (i % 100 == 0) {
            CHECK(array_stack.get(i) == i * 10);
            CHECK(array_stack.size() == i + 1);
        }
    }
}

int main() {
    return test::run_tests({
        {"test_basic_operations", test_basic_operations},
        {"test_bounds_and_exceptions", test_bounds_and_exceptions},
        {"test_resize_behavior", test_resize_behavior},
    });
}
