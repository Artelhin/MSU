//Here methods of analysers will be used in order to check? compile and run a programm in model language
#include <iostream>
#include <fstream>
using namespace std;

int main() {

    ifstream fin;
    char str[50];
    fin.open("../resources/atest.txt");

    /*fin >> str;
    cout << str << endl;
    fin >> str;
    cout << str << endl;
    fin >> str;
    cout << str << endl;
    fin >> str;
    cout << str << endl;
    fin >> str;
    cout << str << endl;*/

    fin.getline(str, 50);
    cout << str << endl;
    cout << "ended" << endl;

    fin.close();

    return 0;
>>>>>>> 109e83144f498ec7cab7ea696a8c3efe6ffd6b3c

}
