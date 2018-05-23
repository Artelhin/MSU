#include "SyntAn.cpp"

using namespace std;

int main() {
    SyntAnalyser SA("atest.txt");
    bool res;

    res = SA.checkAndRun();

    cout << res;

    return 0;
}
