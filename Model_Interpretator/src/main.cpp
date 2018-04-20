//Here methods of analysers will be used in order to check? compile and run a programm in model language
#include "LexAn.cpp"

using namespace std;

void out(Lex l, Scanner s) {
    char d;
    cin >> d;
    cout << endl;
    if(l.get_type() < 18 || l.get_type() == 42)
        cout << "Word: " << s.getWord(l.get_value()) << endl;
    else if (l.get_type() >= 19 && l.get_type() <= 34)
        cout << "Delim: " << s.getDelim(l.get_value()) << endl;
    else if (l.get_type() == LEX_ID)
        cout << "Ident: " << s.getIdent(l.get_value()) << endl;
    else if (l.get_type() == LEX_NUM)
        cout << "Numb: " << l.get_value() << endl;
}

int main() {
    cout<<"Hello"<<endl;
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
