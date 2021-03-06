#ifndef MYSTACK
#define MYSTACK
#include "includings.h"
template <class T, int max_size> class Stack {
    T s[max_size];
    int top;
public:
    Stack(){
        top = 0;
    }
    void reset() { top = 0; }
    void push(T i) {
        if( !is_full() ) {
            s[top] = i;
            top++;
        } else throw new S_exep("Stack is full!");
    }

    T pop() {
        if( !is_empty() ) {
            top--;
            return s[top];
        } else throw new S_exep("Stack is empty!");
    }

    bool is_empty() { return top == 0; }
    bool is_full() { return top == max_size; }
};

#endif
