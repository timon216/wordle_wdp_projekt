#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    // fix Polish character encoding in console (output & input)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    cout << "WORDLE" << endl;

    return 0;
}