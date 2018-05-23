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
    LEX_TRUE,        //14
    LEX_WHILE,       //15
    LEX_WRITE,       //16

    LEX_FIN,         //17

    LEX_SEMICOLON,   //18
    LEX_COMMA,       //19
    LEX_COLON,       //20
    LEX_ASSIGN,      //21
    LEX_LPAREN,      //22
    LEX_RPAREN,      //23
    LEX_EQ,          //24
    LEX_LSS,         //25
    LEX_GTR,         //26
    LEX_PLUS,        //27
    LEX_MINUS,       //28
    LEX_TIMES,       //29
    LEX_SLASH,       //30
    LEX_LEQ,         //31
    LEX_NEQ,         //32
    LEX_GEQ,         //33

    LEX_NUM,         //34
    LEX_ID,          //35
    POLIZ_LABEL,     //36
    POLIZ_ADDRESS,   //37
    POLIZ_GO,        //38
    POLIZ_FGO,       //39
    LEX_QUOTE,       //40
    LEX_STRING,      //41
    LEX_CSTRING,     //42
    LEX_FOR,         //43
    LEX_BREAK,       //44
    LEX_GOTO,        //45
    LEX_STRUCT,      //46
    LEX_TSTRUCT,     //47 for struct needs
    LEX_LABEL,       //48 for goto needs
    LEX_POINT
};

class Lex {
    type_of_lex t_lex;
    int v_lex;
    int line;

public:

    Lex(type_of_lex t, int v = 0, int l = 0) {
        t_lex = t;
        v_lex = v;
        line = l;
    }

    Lex(int l = 0, type_of_lex t = LEX_NULL, int v = 0) {
        t_lex = t;
        v_lex = v;
        line = l;
    }

    type_of_lex get_type() { return t_lex; }
    int get_value() { return v_lex; }
    int get_line() { return line; }

    friend ostream& operator << (ostream &s, Lex l);
};

ostream& operator << (ostream &s, Lex l) {
    s << '(' << l.t_lex << ',' << l.v_lex << ");";
    return s;
}
#endif // LEXEM
