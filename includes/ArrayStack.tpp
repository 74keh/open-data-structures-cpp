#ifndef ARRAY_STACK_TPP
#define ARRAY_STACK_TPP

#include <algorithm>
#include <stdexcept>

#include "ArrayStack.hpp"

template <typename T>
ArrayStack<T>::ArrayStack() : array(nullptr), array_capacity(0), stack_size(0) {
    resize();
}

template <typename T>
ArrayStack<T>::~ArrayStack() {
    delete[] array;
}

template <typename T>
int ArrayStack<T>::size() const {
    return stack_size;
}

template <typename T>
bool ArrayStack<T>::is_empty() const {
    return stack_size == 0;
}

template <typename T>
T ArrayStack<T>::get(int i) const {
    if (i < 0 || i >= stack_size) {
        throw std::out_of_range("get(): index out of range");
    }
    return array[i];
}

template <typename T>
T ArrayStack<T>::set(int i, const T& x) {
    if (i < 0 || i >= stack_size) {
        throw std::out_of_range("set(): index out of range");
    }
    T old = array[i];
    array[i] = x;
    return old;
}

template <typename T>
void ArrayStack<T>::add(int i, const T& x) {
    if (i < 0 || i > stack_size) {
        throw std::out_of_range("add(): index out of range");
    }

    if (stack_size + 1 > array_capacity) {
        resize();
    }

    for (int j = stack_size; j > i; --j) {
        array[j] = array[j - 1];
    }

    array[i] = x;
    ++stack_size;
}

template <typename T>
T ArrayStack<T>::remove(int i) {
    if (i < 0 || i >= stack_size) {
        throw std::out_of_range("remove(): index out of range");
    }

    T removed = array[i];

    for (int j = i; j < stack_size - 1; ++j) {
        array[j] = array[j + 1];
    }

    --stack_size;

    if (array_capacity >= 3 * stack_size) {
        resize();
    }

    return removed;
}

template <typename T>
void ArrayStack<T>::resize() {
    int new_capacity = std::max(1, 2 * stack_size);
    T* new_array = new T[new_capacity];

    for (int i = 0; i < stack_size; ++i) {
        new_array[i] = array[i];
    }

    delete[] array;
    array = new_array;
    array_capacity = new_capacity;
}

#endif  // ARRAY_STACK_TPP