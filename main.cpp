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

// function that converts Polish characters to uppercase
string toUpperCasePolish(string word) {
    string result = "";

    for (size_t i = 0; i < word.size();) {
        // Check for 2-byte Polish uppercase letters
        if (i + 1 < word.size()) {
            string twoChar = word.substr(i, 2);

            if (twoChar == "ą") { result += "Ą"; i += 2; continue; }
            if (twoChar == "ć") { result += "Ć"; i += 2; continue; }
            if (twoChar == "ę") { result += "Ę"; i += 2; continue; }
            if (twoChar == "ł") { result += "Ł"; i += 2; continue; }
            if (twoChar == "ń") { result += "Ń"; i += 2; continue; }
            if (twoChar == "ó") { result += "Ó"; i += 2; continue; }
            if (twoChar == "ś") { result += "Ś"; i += 2; continue; }
            if (twoChar == "ź") { result += "Ź"; i += 2; continue; }
            if (twoChar == "ż") { result += "Ż"; i += 2; continue; }
        }

        // change ASCII uppercase to lowercase
        if (word[i] >= 'a' && word[i] <= 'z') {
            result += char(word[i] - 32);
        }
        // Everything else stays as is
        else {
            result += word[i];
        }

        i++;
    }
    return result;
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

// function that divides a single string into a vector of multiple substrings (letters)
vector<string> wordLetters(const string& word) {
    vector<string> result;
    for (size_t i = 0; i < word.size();) {
        // Check for 2-byte Polish uppercase letters in UTF-8
        if (i + 1 < word.size()) {
            string twoChar = word.substr(i, 2);
            if (twoChar == "Ą" || twoChar == "Ć" || twoChar == "Ę" || twoChar == "Ł" || twoChar == "Ń" || twoChar == "Ó" || twoChar == "Ś" || twoChar == "Ź" ||twoChar == "Ż") {
                result.push_back(twoChar);
                i += 2;
                continue;
            }
        }
        string oneChar = word.substr(i, 1);
        result.push_back(oneChar);
        i++;
    }
    return result;
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

    int guessCounter = 0;

    while(guessCounter < 6)
    {
        cout << "Podaj słowo: ";
        cin >> guessWord;
        string guessUpper = toUpperCasePolish(guessWord);
        while(validateGuessWord(guessUpper, dictionary) != true)
        {
            cout << "Niepoprawne słowo! Podaj nowe: ";
            cin >> guessWord;
            guessUpper = toUpperCasePolish(guessWord);
        }
        if(answerWord == guessUpper)
        {
            cout << "Poprawna odpowiedź!" << endl;
            break;
        }
        else
        {
            guessCounter++;
            cout << "Zła odpowiedź! Pozostało " << 6 - guessCounter << " prób."<< endl;
        }
    }

    return 0;
}