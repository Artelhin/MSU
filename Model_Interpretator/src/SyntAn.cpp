#ifndef GUARD_SYNT
#define GUARD_SYNT
#include "LexAn.cpp"
#include "Stack.cpp"
#include "Poliz.cpp"

using namespace std;


//ТАБЛИЦА ТИПОВ СТРУКТУР
class TableTypeStructs {
    ident *p;
    int size;
    int top;
public:
    TableTypeStructs(int max_size) {
        p = new ident[max_size];
        size = max_size;
        top = 0;
        ident kostyl;
        kostyl.put_type(LEX_NULL);
        p[0] = kostyl;
        top++;
    }

    ~TableTypeStructs() {
        delete []p;
    }

    void add(ident t) {
        p[top] = t;
        top++;
    }

    int get_top() { return top; }

    //возвращает идентификатор по индексу или идентификатор типа LEX_NULL в случае достижения конца таблицы
    ident& operator[] (int k) {
        if (k >= size) throw new S_exep("TTSTR: out of array!");
        if (k == top) {
            return p[0];
        }
        if (k > top) throw new S_exep("TTSTR.operator[]: unidentified ident!");
        return p[k];
    }

    //возвращает номер поля name в структуре, определяемой идентификатором str
    int search_in_struct(ident str, char* name) {
        if (str.get_type() != LEX_TSTRUCT) throw new S_exep("TSTR: not a struct arguement for search_in_struct()!");
        int i = 0;
        while(i < top) {
            if (!strcmp(p[i].get_name(), str.get_name())) break;
            i++;
        }
        if (i == top) throw new S_exep("TTSTR: No such struct!");
        i++;
        while (p[i].get_type() != LEX_TSTRUCT && i < top) {
            if (!strcmp( p[i].get_name() , name )) break;
            i++;
        }
        if (p[i].get_type() == LEX_TSTRUCT || i == top) throw new S_exep("TTSTR: No such field!");
        return i;
    }

    void put(ident id) {
        if (top == size) throw new S_exep("TTSTR: out of memory!");
        p[top] = id;
        top++;
    }

    int search_for_struct(ident str) {
        if (str.get_type() != LEX_TSTRUCT) throw new S_exep("TTSTR: not a struct arguement for search_for_struct()!");
        int i = 0;
        while(i < top) {
            if (!strcmp(p[i].get_name(), str.get_name())) break;
            i++;
        }
        if (i == top) throw new S_exep("TTSTR: No such struct!");
        return i;
    }

    bool isDeclared(char* name, ident str) {
        try {
            search_in_struct(str, name);
            return true;
        } catch (S_exep ex) {
            return false;
        }
    }

};//КОНЕЦ ТАБЛИЦЫ ТИПОВ

TableTypeStructs TTSTR(100);

//ТАБЛИЦА ПЕРЕМЕННЫХ-СТРУКТУР
//переменные создаются на основе структур, описанных в таблице типов структур TTSTR
class TableOfStructs {
    ident *p;
    int size;
    int top;

    void put(ident id) {
        if (top == size) throw new S_exep("TSTR: out of memory!");
        p[top] = id;
        top++;
    }

    //возвращает идентификатор по индексу или идентификатор типа LEX_NULL в случае достижения конца таблицы
    ident& operator[] (int k) {
        if (k >= size) throw new S_exep("TSTR: out of array!");
        if (k == top) {
            return p[0];
        }
        if (k > top) throw new S_exep("TSTR.operator[]: unidentified ident!");
        return p[k];
    }

public:

    TableOfStructs(int max_size) {
        size = max_size;
        top = 0;
        p = new ident[max_size];
        ident kostyl;
        kostyl.put_type(LEX_NULL);
        p[0] = kostyl;
        top++;
    }
    ~TableOfStructs() { delete []p; }


    //Инициализирует последовательность полей, основываясь на описании структуры str в TTSTR
    int init(ident str) {
        if (str.get_type() != LEX_TSTRUCT) throw new S_exep("TSTR: Not a structure type in init arg!");
        int struct_top = top;
        ident temp;
        put(str);
        int i = TTSTR.search_for_struct(str);
        i++;
        while ( TTSTR[i].get_type() != LEX_NULL || TTSTR[i].get_type() != LEX_TSTRUCT) {
            put(TTSTR[i]);
            i++;
        }
        return struct_top;
    }

    ident& get_field(ident str, char* name) {
        if (str.get_type() != LEX_STRUCT) throw new S_exep("TSTR.get_field(): Not a struct variable, no fields here!");
        ident definition = p[str.get_value()];
        int base = TTSTR.search_in_struct(definition, name);
        base = str.get_value() + base;
        return p[base];
    }

}; //КОНЕЦ ТАБЛИЦЫ ПЕРЕМЕННЫХ-СТРУКТУР

TableOfStructs TSTR(300);

//СИНТАКСИЧЕСКИЙ АНАЛИЗАТОР
class SyntAnalyser {
    Scanner lexParser;
    Lex cur_lex;
    type_of_lex c_type;
    int c_val;
    Stack <type_of_lex, 100> st_lex;
    Stack <int, 100> st_struct;


public:

    Poliz prog;

    SyntAnalyser(const char* path) : prog(1000), lexParser(path) {
        cout << "SyntAnalyser initializing...";
        cout << "SyntAnalyser initialized." << endl;
    }

    void gl() {
        cur_lex = lexParser.get_lex();
        c_val = cur_lex.get_value();
        c_type = cur_lex.get_type();
    }

    //check double declaration
    void check_dd() {
        if (TID[c_val].isDeclared()) throw new S_exep("Double declaration!");
        TID[c_val].put_declared();
    }

    //check declaration
    bool check_id() {
        return TID[c_val].isDeclared();
    }

    //Заносит запись о новом типе структуре в таблицу типов структур TTSTR
    void dec_struct() {
        TID[c_val].put_type(LEX_TSTRUCT);
        TID[c_val].put_declared();
        TID[c_val].put_value(TTSTR.get_top());
        TTSTR.put(TID[c_val]);
    }

    void eq_types() {
        type_of_lex t1, t2, op, res;

        t2 = st_lex.pop();
        op = st_lex.pop();
        t1 = st_lex.pop();
        int base;

        if (op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) { // - * /
            if (t1 == t2 && t1 == LEX_INT) {
                res = LEX_INT;
            }
        } else if (op == LEX_PLUS) { // +
            if (t1 == t2) {
                if (t1 == LEX_STRING) res = LEX_STRING;
                else if (t1 == LEX_INT) res = LEX_INT;
                else throw new S_exep("EQ_TYPES: Wrong types for + operator!");
            } else throw new S_exep("EQ_TYPES: Different types for + operator!");
        } else if (op == LEX_EQ || op == LEX_NEQ || op == LEX_GEQ || op == LEX_LEQ || op == LEX_LSS || op == LEX_GTR) {
            if (t1 == t2 && (t1 == LEX_INT || t1 == LEX_BOOL)) res = LEX_BOOL;
            else throw new S_exep("EQ_TYPES: Wrong types for compare operator!");
        } else if (op == LEX_EQ) {
            if (t1 == t2) res = t1;
            else throw new S_exep("EQ_TYPES: Wrong types in assigning");
        } else if (op == LEX_AND || op == LEX_OR) {
            if (t1 == t2 && t1 == LEX_BOOL) res = LEX_BOOL;
            else throw new S_exep("EQ_TYPES: Wrong types for logic operator!");
        } else throw new S_exep("EQ_TYPES: Stack error!");
        st_lex.push(res);
        prog.put_lex(Lex(op));
    }

    void eq_bool() {
        type_of_lex t;
        t = st_lex.pop();
        if (t != LEX_BOOL) throw new S_exep("EQ_BOOL: Not bool!");
    }

    void check_unar() {
        type_of_lex t = st_lex.pop(), op = st_lex.pop();
        if (t != LEX_BOOL) throw new S_exep("CHECK_UNAR: Not bool after \"not\"!");
    }

    void check_id_in_read() {
        ident id = TID[c_val];
        if(!id.isDeclared()) throw new S_exep("CHECK_IN_READ: Undeclared identifier!");
        if(id.get_type() == LEX_STRUCT) throw new S_exep("CHECK_IN_READ: Cannot read structures!");
    }

    bool checkAndRun() {
        gl();
        try{
            H();
        } catch (S_exep *ex) {
            cout << "SyntAnalyser: ";
            ex->out();
            cout << endl;
            return false;
        }
        cout << "SyntAnalyser: Compilation finished successfully." << endl;
        return true;
    }

    void H() {
        cout << "H" << endl;
        st_lex.reset();
        if (c_type != LEX_PROGRAM) throw new S_exep("\"program\" expected");
        gl();
        S();
        if(c_type != LEX_BEGIN) throw new S_exep("\"{\" expected before program body");
        gl();
        P();
        if(c_type != LEX_END) throw new S_exep("\"}\" expected after program body");
        if(c_type != LEX_FIN) throw new S_exep("\"EOF\" expected");
    }

    void S() {
        cout << "S" << endl;
        if (c_type != LEX_STRUCT) return;
        gl();
        if (c_type != LEX_ID) throw new S_exep("Identifier expected after \"struct\"");
        check_dd();
        dec_struct();
        ident str = TID[c_val];
        gl();
        if (c_type != LEX_BEGIN) throw new S_exep("\"{\" expected before struct body");
        Q1(str);
        if (c_type != LEX_END) throw new S_exep("\"}\" expected after struct body");
        gl();
        if (c_type != LEX_SEMICOLON) throw new S_exep("; expected after struct declaration");
        gl();
        S();
    }

    void Q1(ident str) {
        cout << "Q1" << endl;
        if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING) {
            type_of_lex temp = c_type;
            gl();
            D1(temp, str);
            if(c_type != LEX_SEMICOLON) throw new S_exep("; expected after field declaration!");
            gl();
            Q1(str);
        }
    }

    //Добавляет поле типа temp в таблицу типов структур TTSTR
    void D1(type_of_lex type, ident str) {
        cout << "D1" << endl;
        if (c_type != LEX_ID) throw new S_exep("Identifier expected!");
        ident temp;
        temp.put_type(type);
        temp.put_name(TID[c_val].get_name());
        temp.put_declared();
        if (TTSTR.isDeclared(temp.get_name(), str)) throw new S_exep("Double declaration of fields!");
        TTSTR.put(temp);
        gl();

    }

    void P() {
        cout << "P" << endl;
        if (c_type == LEX_END) return;
        Q();//declarations
        O();//commands
    }

    void Q() {
        cout << "Q" << endl;
        if (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING) {
            type_of_lex temp = c_type;
            gl();
            D(temp);
            if(c_type != LEX_SEMICOLON) throw new S_exep("; expected after variable declaration!");
        } else if (c_type == LEX_ID) {
            if (check_id() )
                if (TID[c_val].get_type() == LEX_TSTRUCT) {
                    int nStruct = c_val; // индекс типа в TTSTR
                    gl();
                    D2(TID[nStruct]);
                    if (c_type != LEX_SEMICOLON) throw new S_exep("; expected after struct-variable declaration");
                } else return;
            else return;
        }
        gl();
        Q();
    }

    //Инициализация переменных типа СТРУКТУРА
    void D2(ident str) {
        cout << "D2" << endl;
        if (c_type != LEX_ID) throw new S_exep("Identifier expected after struct type!");
        TID[c_val].put_type(LEX_STRUCT);
        TID[c_val].put_value(TSTR.init(str));
        gl();
        while (c_type == LEX_COMMA) {
            if (c_type != LEX_ID) throw new S_exep("Identifier expected after struct type!");
            TID[c_val].put_type(LEX_STRUCT);
            TID[c_val].put_value(TSTR.init(str));
            gl();
        }
    }

    void D(type_of_lex type) {
        cout << "D" << endl;
        if (!c_type == LEX_ID) throw new S_exep("Identifier expected!");
        TID[c_val].put_declared();
        TID[c_val].put_type(type);
        st_lex.push(type);
        gl();
        if (c_type == LEX_ASSIGN) {
            gl();
            E();
            eq_types(); //check operands
        }
        while (c_type == LEX_COMMA) {
            gl();
            if (!c_type == LEX_ID) throw new S_exep("Identifier expected!");
            TID[c_val].put_declared();
            TID[c_val].put_type(type);
            st_lex.push(type);
            gl();
            if (c_type == LEX_ASSIGN) {
                gl();
                E();
                eq_types(); //check operands
            }
        }
    }

    void O() {
        cout << "O" << endl;
        if (c_type == LEX_END) return;
        A();
        O();
    }

    void A(bool inCycle = false, int pl4 = 0) {

        int pl0, pl1, pl2, pl3;
        cout << "A" << endl;
        if (c_type == LEX_IF) {
            gl();
            E();
            eq_bool();//check for correct type
            pl2 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_FGO));
            gl();
            A();
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
            if (c_type == LEX_ELSE) {
                pl3 = prog.get_free();
                prog.blank();
                prog.put_lex(POLIZ_GO);
                gl();
                A();
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
            }
        } // END IF
        else if (c_type == LEX_WHILE) {
            pl0 = prog.get_free();
            gl();
            E();
            eq_bool();
            pl1 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_FGO));
            A(true, pl1);
            prog.put_lex(Lex(POLIZ_LABEL, pl0));
            prog.put_lex(Lex(POLIZ_GO));
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
        } // END WHILE
        else if (c_type == LEX_FOR) {
            gl();
            if (c_type == LEX_LPAREN) {
                E();
                if (c_type != LEX_SEMICOLON) throw new S_exep("; expected if for()!");
                pl0 = prog.get_free();//expression
                E();
                eq_bool();
                pl1 = prog.get_free();//label after expressoin to the place after cycle
                prog.blank();
                prog.put_lex(Lex(POLIZ_FGO));
                prog.blank(); // pl1+2
                prog.put_lex(Lex(POLIZ_GO));
                if (c_type != LEX_SEMICOLON) throw new S_exep("; expected if for()!");
                pl2 = prog.get_free(); // expression which is after cycle
                E();
                prog.put_lex(Lex(POLIZ_LABEL, pl0));
                prog.put_lex(Lex(POLIZ_GO));
                if (c_type != LEX_RPAREN) throw new S_exep(") expected after expressoin in for!");
                A(true, pl1);
                prog.put_lex(Lex(POLIZ_LABEL, pl1+2));
                prog.put_lex(Lex(POLIZ_GO));
            } else throw new S_exep("(<E>;<E>;<E>) expected after for!");
        } // END FOR
        else if (c_type == LEX_READ) {
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                if (c_type == LEX_ID) {
                    check_id_in_read();
                    prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
                    gl();
                }
                else throw new S_exep("Identifier expected in read() function!");
                if (c_type == LEX_RPAREN) {
                    gl();
                    prog.put_lex(Lex(LEX_READ));
                } else throw new S_exep(") expected!");
            } else throw new S_exep("(<identifier>) expected after read!");
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else throw new S_exep("; expected!");
        } // END READ
        else if (c_type == LEX_WRITE) {
            gl();
            if (c_type == LEX_LPAREN) {
                gl();
                E();
                if (c_type == LEX_RPAREN) {
                    gl();
                    prog.put_lex(Lex(LEX_WRITE));
                } else throw new S_exep(") expected!");
            } else throw new S_exep("(<expression>) expected after write!");
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else throw new S_exep("; expected!");
        } // END WRITE
        else if (c_type == LEX_ID) {
            if ( !check_id() ) {
                int labelTemp = c_val;
                gl();
                if (c_type == LEX_COLON) {
                    TID[labelTemp].put_declared();
                    TID[labelTemp].put_value(prog.get_free());
                    TID[labelTemp].put_assigned();
                    TID[labelTemp].put_type(LEX_LABEL);
                    gl();
                    A();
                } else throw new S_exep("Undeclared identifier!");
            } else {
                if ( TID[c_val].get_type() == LEX_TSTRUCT ) throw new S_exep("Types not allowed for use here!");
                if ( TID[c_val].get_type() == LEX_LABEL ) throw new S_exep("Label is already defined!");
                E();
            }
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else throw new S_exep("; expected!");
        } // END ID
        else if (c_type == LEX_GOTO) {
            pl0 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO));
            gl();
            if (c_type == LEX_ID) {
                prog.put_lex(Lex(POLIZ_ADDRESS, c_val), pl0);
            } else throw new S_exep("Identifier expected after goto!");
        } // END GOTO
        else if (c_type == LEX_BREAK) {
            if(!inCycle) throw new S_exep("\"break\" only allowed in cycles!");
            prog.put_lex(Lex(LEX_LABEL, pl4));
            prog.put_lex(Lex(POLIZ_GO));
        }
        else if (c_type == LEX_BEGIN) { // complex orerator
            gl();
            while(c_type != LEX_END) A();
            gl();
        } // END COMPLEX
        else E();
    }

    bool is_var = false;
    bool pushed_struct = false;

    void E() {

        cout << "E" << endl;

        E1();

        while(c_type = LEX_ASSIGN) {
            if(!is_var) throw new S_exep("Can assign only to variables!");
            st_lex.push(c_type);
            gl();
            E1();
            eq_types();
            prog.put_lex(Lex(LEX_ASSIGN));
        }
    }

    void E1() {

        cout << "E1" << endl;
        E2();
        while (c_type == LEX_EQ || c_type == LEX_NEQ || c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_LSS || c_type == LEX_GTR) {
            st_lex.push(c_type);
            gl();
            E2();
            eq_types();
        }
    }

    void E2() {
        cout << "E2" << endl;
        E3();
        while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
            st_lex.push(c_type);
            gl();
            E3();
            eq_types();
        }
    }

    void E3() {
        cout << "E3" << endl;
        E4();
        while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
            st_lex.push(c_type);
            gl();
            E4();
            eq_types();
        }
    }

    void E4() {
        cout << "E4" << endl;
        type_of_lex last_type = c_type;
        int last_val = c_val;

        switch(c_type){
            case LEX_ID: {
                is_var = true;
                if(!TID[c_val].isDeclared()) throw new S_exep("Undefined identifier!");
                if(TID[c_val].get_type() == LEX_STRUCT) { //это значит, что перменная это структура, и в её value находится индекс идентификатора типа структуры в TSTR
                    pushed_struct = true;
                    last_type = TID[c_val].get_type();
                    st_struct.push(TID[c_val].get_value());
                }
                st_lex.push(last_type);
                prog.put_lex(cur_lex);
                gl();
                if (c_type == LEX_POINT) {
                    if(!pushed_struct) throw new S_exep("Not a structure - no fields to access!");
                    int base = st_struct.pop();
                    gl();
                    if (c_type != LEX_ID) throw new S_exep("Expected identifier after \".\"!");
                    st_lex.push( TSTR.get_field( TID[last_val] , TID[c_val].get_name()).get_type() );//получение типа поля структуры по имени поля и идентификатору переменной-структуры
                    prog.put_lex(cur_lex);
                    prog.put_lex(Lex(LEX_POINT));
                }
            }
            break;

            case LEX_NUM: {
                st_lex.push(LEX_INT);
                prog.put_lex(cur_lex);
            }
            break;

            case LEX_CSTRING: {
                st_lex.push(LEX_STRING);
                prog.put_lex(cur_lex);
            }
            break;

            case LEX_TRUE: {
                st_lex.push(LEX_BOOL);
                prog.put_lex(cur_lex);
            }
            break;

            case LEX_FALSE: {
                st_lex.push(LEX_BOOL);
                prog.put_lex(cur_lex);
            }
            break;

            case LEX_NOT: {
                st_lex.push(c_type);
                gl();
                E();
                check_unar();
                return;
            }
            break;

            case LEX_MINUS: {
                prog.put_lex(Lex(LEX_NUM, 0));
                st_lex.push(LEX_INT);
                gl();
                E();
                eq_types();
                prog.put_lex(Lex(LEX_MINUS));
                return;
            }
            break;

            case LEX_LPAREN: {
                gl();
                E();
                if (c_type != LEX_RPAREN) throw new S_exep("Expected ) after expression!");
            }
            break;

            default: throw new S_exep("Unknown exep in E4()!");
        }
        is_var = false;
        gl();
    }
};
#endif
