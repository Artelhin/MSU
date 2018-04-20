#ifndef LEXAN
#define LEXAN
#include "includings.h"

using namespace std;

tabl_ident TID(100);

class tabl_cstr {
    char** p;
    int size;
    int top;

public:
    tabl_cstr(int s) {
        size = s;
        p = new char *[size];
        top = 1;
    }
    ~tabl_cstr() {
        for(int i = 0; i< size; i++) delete [](p[i]);
        delete []p;
    }

    char* operator[] (int k) {
        if (k > size) {
            S_exep ex = S_exep("tabl_cstr.operator[]: out of array");
            throw ex;
        }
        return p[k];
    }

    int put(const char* buf) {
        for(int j = 1; j < top; j++)
            if (!strcmp(buf,p[j]))
                return j;
        p[top] = new char[strlen(buf) + 1];
        strcpy(p[top], buf);
        top++;
        return top-1;
    }

} TCS(100);

class Scanner {
    enum state {H, IDENT, NUMB, COM, ALE, DELIM, NEQ, STR };
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
        if (buf_top == 80) throw S_exep("Analizer's buffer overloaded");
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
        if (fp == NULL) throw S_exep("LexicalAnalyzer: file not found");
        CS = H;
        clear();
        line = 1;
        symb = 0;
        gc();
        if (c == EOF) throw S_exep("LexicalAnalyzer: empty file");
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
    char* getConstStr(int k) {
        return TCS[k];
    }

    Lex get_lex();
};

char* Scanner::TW[] = {
    "",                //    0  not used
    "and",             //    1
    "boolean",         //    2
    "do",              //    3
    "else",            //    4
    "if",              //    5
    "false",           //    6
    "int",             //    7
    "not",             //    8
    "or",              //    9
    "program",         //    10
    "read",            //    11
    "true",            //    12
    "while",           //    13
    "write",           //    14
    "string",          //    15
    "for",             //    16
    "break",           //    17
    "goto",            //    18
    "struct",          //    19
    "label",           //    20
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
    int tempLine;
    int tempSymb;
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
                    return Lex(line, LEX_FIN);
                else if (c == '!') {
                    clear();
                    add();
                    gc();
                    CS = NEQ;
                }
                else if (c == '"') {
                    clear();
                    tempLine = line;
                    tempSymb = symb;
                    gc();
                    CS = STR;
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
                    return Lex(line, words[j], j);
                else {
                    j = TID.put(buf);
                    return Lex(line, LEX_ID, j);
                }
                break;
            case NUMB:
                //check();
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                    gc();
                }
                else
                    return Lex(line, LEX_NUM, d);
                break;
            case COM:
                if (c == '/') {
                    gc();
                    while(c != '\n') {
                        if (c == EOF) return Lex(line, LEX_FIN);
                        gc();
                    }
                    CS = H;
                }
                else if (c == '*') {
                    gc();
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
                    return Lex(line, dlms[j], j);
                }
                else {
                    j = look(buf, TD);
                    return Lex(line, dlms[j], j);
                }
                break;
            case NEQ:
                //check();
                if (c == '=') {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(line, LEX_NEQ, j);
                }
                else {
                    L_exep ex("Unexpected '!'", line, symb);
                }
                break;
            case STR:
                while(c != '"') {
                    add();
                    gc();
                    if (c == EOF) {
                        L_exep ex("Unclosed \" ",tempLine,tempSymb);
                        throw ex;
                    }
                }
                gc();
                j = TCS.put(buf);
                return Lex(line, LEX_CSTRING, j);
            case DELIM:
                //check();
                clear();
                add();
                if (j = look(buf, TD)) {
                    gc();
                    return Lex(line, dlms[j], j);
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
