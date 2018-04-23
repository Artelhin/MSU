#ifndef GUARD_SYNT
#define GUARD_SYNT
#include "LexAn.cpp"

using namespace std;

class SyntAnalyser {
    Scanner lexParser;
    //Runner runner; //NEEDS IMPLEMENTATION
    Lex cur;

public:
    SyntAnalyser(const char* path) {
        cout << "SyntAnalyzer initializing..."
        try {
            lexParser = Scanner(path);
        } catch(S_exep ex) {
            cout << "Error occured on LexicalAnalyzer initializing: " << ex << endl;
            throw S_exep("SyntAnalyzer: Failed to initialize LexicalAnalyzer");
        }
        /*
        try {
            runner = Runner();
        } catch(S_exep ex) {
            cout << "Error occured on Runner initializing: " << ex << endl;
            throw S_exep("SyntAnalyzer: Failed to initialize Runner");
        }
        */
        cout << "SyntAnalyser initialized." << endl;
    }

    bool checkAndRun() {
        cur = lexParser.get_lex();
        if (cur == LEX_PROGRAM) {
            cur = lexParser.get_lex();
            return program();
        }
        return false;
    }

    bool program() {
        if (cur == LEX_BEGIN) {
            cur = lexParser.get_lex();
            innerBlock();
            cur = lexParser.get_lex();
            if (cur == LEX_END) return true;
            return false;
        }
    }

    void innerBlock() {

    }
}
