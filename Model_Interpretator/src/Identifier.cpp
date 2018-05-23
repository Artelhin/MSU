#ifndef GUARD_IDENT
#define GUARD_IDENT
#include "includings.h"

using namespace std;

class ident {
    char* name;
    bool declared;
    type_of_lex type;
    bool assigned;
    int value;

public:
    ident() {
        declared = false;
        assigned = false;
    }

    char* get_name() {
        return name;
    }

    void put_name(const char* n) {
        name = new char[strlen(n) + 1];
        strcpy(name,n);
    }

    bool isDeclared() {
        return declared;
    }

    void put_declared() {
        declared = true;
    }

    bool isAssigned() {
        return assigned;
    }

    void put_assigned() {
        assigned = true;
    }

    int get_value() {
        return value;
    }

    void put_value(int v) {
        value = v;
    }

    void put_type(type_of_lex t) { type = t; }

    type_of_lex get_type() { return type; }
};

class tabl_ident {
    ident* p;
    int size;
    int top;

public:
    tabl_ident(int max_size) {
        p = new ident[size=max_size];
        top = 1;
    }

    ~tabl_ident() {
        delete []p;
    }

    ident& operator[] (int k) {
        if (k > size) {
            S_exep ex = S_exep("tabl_ident.operator[]: out of array");
            throw ex;
        }
        return p[k];
    }

    int put(const char* buf) {
        for(int j = 1; j < top; j++)
            if (!strcmp(buf,p[j].get_name()))
                return j;
        p[top].put_name(buf);
        top++;
        return top-1;
    }
};

#endif
