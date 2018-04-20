#ifndef LEXEM
#define LEXEM
#include <iostream>

using namespace std;

enum type_of_lex {
    LEX_NULL,        //0
    LEX_AND,         //1
    LEX_BEGIN,       //2
    LEX_BOOL,        //3
    LEX_DO,          //4
    LEX_ELSE,        //5
    LEX_END,         //6
    LEX_IF,          //7
    LEX_FALSE,       //8
    LEX_INT,         //9
    LEX_NOT,         //10
    LEX_OR,          //11
    LEX_PROGRAM,     //12
    LEX_READ,        //13
    LEX_THEN,        //14
    LEX_TRUE,        //15
    LEX_WHILE,       //16
    LEX_WRITE,       //17

    LEX_FIN,         //18

    LEX_SEMICOLON,   //19
    LEX_COMMA,       //20
    LEX_COLON,       //21
    LEX_ASSIGN,      //22
    LEX_LPAREN,      //23
    LEX_RPAREN,      //24
    LEX_EQ,          //25
    LEX_LSS,         //26
    LEX_GTR,         //27
    LEX_PLUS,        //28
    LEX_MINUS,       //29
    LEX_TIMES,       //30
    LEX_SLASH,       //31
    LEX_LEQ,         //32
    LEX_NEQ,         //33
    LEX_GEQ,         //34

    LEX_NUM,         //35
    LEX_ID,          //36
    POLIZ_LABEL,     //37
    POLIZ_ADDRESS,   //38
    POLIZ_GO,        //39
    POLIZ_FGO,       //40
    LEX_QUOTE,       //41
    LEX_STRING       //42
};

class Lex {
    type_of_lex t_lex;
    int v_lex;

public:
    Lex(type_of_lex t = LEX_NULL, int v = 0) {
        t_lex = t;
        v_lex = v;
    }

    type_of_lex get_type() { return t_lex; }
    int get_value() { return v_lex; }

    friend ostream& operator << (ostream &s, Lex l);
};

ostream& operator << (ostream &s, Lex l) {
    s << '(' << l.t_lex << ',' << l.v_lex << ");";
    return s;
}
#endif // LEXEM
