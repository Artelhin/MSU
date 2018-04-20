#ifndef MYEXEP
#define MYEXEP
#include <iostream>
#include <string.h>

using namespace std;

class S_exep {
    char* info;

public:
    S_exep(const char* str) {
        info = new char[strlen(str) + 1];
        strcpy(info, str);
    }

friend ostream& operator << (ostream &s, S_exep ex);
};

ostream& operator << (ostream &s, S_exep ex) {
    s << ex.info;
}

class L_exep {
    char* info;
    int line;
    int symb;

public:
    L_exep(const char* str, int l, int s) {
        info = new char[strlen(str) + 1];
        strcpy(info, str);
        line = l;
        symb = s;
    }

friend ostream& operator << (ostream &s, L_exep ex);
};

ostream& operator << (ostream &s, L_exep ex) {
    s << "LexicalAnalyzer:" << ex.line << ':' << ex.symb << ": " << ex.info;
    return s;
}

#endif
