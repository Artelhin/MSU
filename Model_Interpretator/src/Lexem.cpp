#include <iostream>

using namespace std;

class Lexem {
    lex_type type;
    int value;
public:
    Lexem(lex_type t = LEX_NULL, int v = 0) {
        type = t;
        value = v;
    }

    lex_type getType() {return type;}
    int getValue() {return value;}
    friend ostream& operator << (ostream &s, Lex l);
};

ostream& operator << (ostream &s, Lex l) {
    s << '(' << l.type << ',' << l.value << ");";
    return s;
}
