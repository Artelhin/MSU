#ifndef GUARD_SYNT
#define GUARD_SYNT
#include "LexAn.cpp"

using namespace std;

class SyntAnalyser {
    Scanner lexParser;

public:
    SyntAnalyser(const char* path) {
        cout << "SyntAnalyzer initializing..."
        try {
            lexParser = Scanner(path);
        } catch(S_exep ex) {
            cout << "Error occured on LexicalAnalyzer initializing: " << ex << endl;
            throw S_exep("SyntAnalyzer: Failed to initialize LexicalAnalyzer");
        }
        cout << "SyntAnalyser initialized" << endl;
    }
}
