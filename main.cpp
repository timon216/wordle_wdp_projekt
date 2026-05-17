#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <random>

using namespace std;

// function that loads words from a file
vector<string> loadWordsFromFile(const string& filename) {
    vector<string> words;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Błąd: Nie można otworzyć pliku " << filename << endl;
        return words;
    }

    string word;
    while (getline(file, word)) {
        if (!word.empty()) {
            words.push_back(word);
        }
    }

    file.close();
    return words;
}

// function that returns a random word from a list
string getRandomWord(const vector<string>& words) {
    if (words.empty()) return string();

    // static RNG seeded once per program using random_device
    static random_device rd;
    static mt19937 rng(rd());

    uniform_int_distribution<size_t> dist(0, words.size() - 1);
    return words[dist(rng)];
}

int main() {
    // fix Polish character encoding in console (output & input)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // load dictionaries
    vector<string> dictionary = loadWordsFromFile("../dictionary.txt");
    vector<string> answerWordList = loadWordsFromFile("../answer_word_list.txt");

    if (dictionary.empty() || answerWordList.empty()) {
        cerr << "Błąd: Nie udało się wczytać słowników!" << endl;
        return 1;
    }

    // get random word from answerWordList
    string answerWord = getRandomWord(answerWordList);

    return 0;
}