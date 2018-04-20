//Here methods of analysers will be used in order to check? compile and run a programm in model language
#include "LexAn.cpp"

using namespace std;

void out(Lex l, Scanner s) {
    if((l.get_type() < 17 || l.get_type() == 41 || l.get_type() >= 43) && (l.get_type() != 2 && l.get_type() != 6))
        cout << " Word: '" << s.getWord(l.get_value()) << "'" << endl;
    else if ((l.get_type() >= 18 && l.get_type() <= 33) || l.get_type() == 40)
        cout << " Delim: '" << s.getDelim(l.get_value()) << "'" << endl;
    else if (l.get_type() == LEX_ID)
        cout << " Ident: '" << TID[l.get_value()].get_name() << "'" << endl;
    else if (l.get_type() == LEX_NUM)
        cout << " Numb: '" << l.get_value() << "'" << endl;
    else if (l.get_type() == LEX_FIN)
        cout << " EOF" << endl;
    else if (l.get_type() == LEX_CSTRING)
        cout << " Const String: " << TCS[l.get_value()] << endl;
}

int main() {
    Scanner LexAnal("atest.txt");
    Lex l;
    try {
        do {
            l = LexAnal.get_lex();
            cout << l;
            out(l, LexAnal);
        } while(l.get_type() != LEX_FIN);
    } catch(S_exep ex) {
        cout << ex;
    } catch(L_exep ex) {
        cout << ex;
    }
    return 0;

}
