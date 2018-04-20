#ifndef LEXAN
#define LEXAN
#include "includings.h"

using namespace std;

class Scanner {
    enum state {H, IDENT, NUMB, COM, ALE, DELIM, NEQ };
    static char* TW[];
    static type_of_lex words[];
    static char* TD[];
    static type_of_lex dlms[];
    state CS;
    FILE* fp;
    char c;
    char buf[80];
    int buf_top;
    int line;
    int symb;

    void check() {
        cout << "CS is: " << CS << endl << "Symb is: '" << c << "'\n"<< endl;
    }

    void clear() {
        buf_top = 0;
        for(int j = 0; j < 80; j++)
            buf[j] = '\0';
    }

    void add() {
        buf[buf_top++] = c;
    }

    int look(const char* buf, char** list) {
        int i = 0;
        while(list[i]) {
            if(!strcmp(buf, list[i]))
                return i;
            i++;
        }
        return 0;
    }

    void gc() {
        c = fgetc(fp);
        //cout << c << endl;
        switch (c) {
            case '\n':
                line++;
                symb = 0;
                break;
            case '\t':
            case '\r': break;
            default: symb++;
        }
    }

public:

    Scanner(const char* path) {
        fp = fopen(path, "r");
        CS = H;
        clear();
        line = 1;
        symb = 0;
        gc();
    }

    char* getWord(int k) {
        return TW[k];
    }
    char* getIdent(int k) {
        return TID[k].get_name();
    }
    char* getDelim(int k) {
        return TD[k];
    }

    Lex get_lex();
};

char* Scanner::TW[] = {
    "",                //    0  not used
    "and",             //    1
    "bool",            //    2
    "do",              //    3
    "else",            //    4
    "if",              //    5
    "false",           //    6
    "int",             //    7
    "not",             //    8
    "or",              //    9
    "program",         //    10
    "read",            //    11
    "then",            //    12
    "true",            //    13
    "while",           //    14
    "write",           //    15
    "string",          //    16
    NULL
};

char * Scanner:: TD[] = {
    "",             //    0
    ";",            //    1
    ",",            //    2
    ":",            //    3
    ":=",           //    4
    "(",            //    5
    ")",            //    6
    "=",            //    7
    "<",            //    8
    ">",            //    9
    "+",            //    10
    "-",            //    11
    "*",            //    12
    "/",            //    13
    "<=",           //    14
    "=",            //    15
    ">=",           //    16
    "{",            //    17
    "}",            //    18
    "\"",           //    19
    NULL
};

tabl_ident TID(100);

type_of_lex Scanner::words[] = {
    LEX_NULL,
    LEX_AND,
    LEX_BOOL,
    LEX_DO,
    LEX_ELSE,
    LEX_IF,
    LEX_FALSE,
    LEX_INT,
    LEX_NOT,
    LEX_OR,
    LEX_PROGRAM,
    LEX_READ,
    LEX_THEN,
    LEX_TRUE,
    LEX_WHILE,
    LEX_WRITE,
    LEX_STRING,
    LEX_NULL
};

type_of_lex Scanner::dlms[] = {
    LEX_NULL,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_EQ,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_LEQ,
    LEX_NEQ,
    LEX_GEQ,
    LEX_BEGIN,
    LEX_END,
    LEX_QUOTE,
    LEX_NULL
};

Lex Scanner::get_lex() {
    int d, j;
    CS = H;
    do {
        switch(CS) {
            case H:
                //check();
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                    //cout << "skip '" << c << "'\n";
                    gc();
                }
                else if (isalpha(c)) {
                    clear();
                    add();
                    gc();
                    CS = IDENT;
                }
                else if(isdigit(c)) {
                    d = c - '0';
                    gc();
                    CS = NUMB;
                }
                else if(c == '/') {
                    gc();
                    CS = COM;
                }
                else if (c == ':' || c == '<' || c == '>') {
                    clear();
                    add();
                    gc();
                    CS = ALE;
                }
                else if (c == EOF)
                    return Lex(LEX_FIN);
                else if (c == '!') {
                    clear();
                    add();
                    gc();
                    CS = NEQ;
                }
                else
                    CS = DELIM;
                break;
            case IDENT:
                //check();
                if (isalpha(c) || isdigit(c)) {
                    add();
                    gc();
                }
                else if (j = look(buf, TW))
                    return Lex(words[j], j);
                else {
                    j = TID.put(buf);
                    return Lex(LEX_ID, j);
                }
                break;
            case NUMB:
                //check();
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                    gc();
                }
                else
                    return Lex(LEX_NUM, d);
                break;
            case COM:
                if (c == '/') {
                    gc();
                    while(c != '\n') {
                        if (c == EOF) return Lex(LEX_FIN);
                        gc();
                    }
                    CS = H;
                }
                else if (c == '*') {
                    while (c != '*') {
                        if (c == EOF) {
                            L_exep ex("Sudden EOF in the '/*' commentary",line, symb);
                            throw ex;
                        }
                        gc();
                    }
                    gc();
                    if (c != '/') {
                        L_exep ex = L_exep("Expected '/' after '*'", line, symb);
                        throw ex;
                    }
                    gc();
                    CS = H;
                }
                break;
            case ALE:
                //check();
                if (c == '=') {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                else {
                    j = look(buf, TD);
                    return Lex(dlms[j], j);
                }
                break;
            case NEQ:
                //check();
                if (c == '=') {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j);
                }
                else {
                    L_exep ex("Unexpected '!'", line, symb);
                }
                break;
            case DELIM:
                //check();
                clear();
                add();
                if (j = look(buf, TD)) {
                    gc();
                    return Lex(dlms[j], j);
                }
                else {
                    char* str = new char[20];
                    strcpy(str, "Unexpected '");
                    strcat(str, &c);
                    strcat(str, "'");
                    L_exep ex(str, line, symb);
                    throw ex;
                }
                break;
        }       //end switch
    } while (true);

}

#endif
