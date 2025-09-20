#ifndef ARRAY_STACK_HPP
#define ARRAY_STACK_HPP

#include <stdexcept>

template <typename T>
class ArrayStack {
   private:
    T* array;
    int array_capacity;
    int stack_size;

    void resize();

   public:
    ArrayStack();
    ~ArrayStack();

    int size() const;
    bool is_empty() const;

    T get(int i) const;
    T set(int i, const T& x);
    void add(int i, const T& x);
    T remove(int i);
};

#include "ArrayStack.tpp"

#endif  // ARRAY_STACK_HPP