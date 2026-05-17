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

size_t countLetters(const string& word) {
    size_t count = 0;
    for (unsigned char c : word) {
        // count bytes that are NOT continuation bytes (10xxxxxx)
        if ((c & 0xC0) != 0x80) {
            count++;
        }
    }
    return count;
}

bool validateGuessWord(string guess, const vector<string>& words) {
    int length = countLetters(guess);

    if (length != 5) {
        cout << "Twoje słowo musi mieć 5 liter" << endl;
        return false;
    }
    for (int i = 0; i < words.size(); i++) {
        if (guess == words[i]) {
            return true;
        }
    }
    return false;
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

    string guessWord;
    cout << "Podaj słowo: ";
    cin >> guessWord;

    cout << validateGuessWord(guessWord, dictionary) << endl;

    return 0;
}