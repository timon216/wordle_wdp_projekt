#include <iostream>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <array>

// define ANSI escape sequence fragments to improve code readability
#define Start "\x1b["
#define textColor "38;2;"
#define backgroundColor "48;2;"
#define correctGreen "8;199;30"
#define letteringWhite "255;255;255"
#define wrongGrey "80;80;80"
#define elsewhereYellow "212;157;4"
#define boldText "1"
#define End "m"
#define Reset "\x1b[0m"
#define patrioticRed "255;0;0"
#define Tab "\t\t\t"

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
        // Check for 2-byte Polish lowercase letters
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

        // change ASCII lowercase to uppercase
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

bool validateGuessWord(const string& guess, const unordered_set<string>& words) {
    int length = countLetters(guess);

    if (length != 5) {
        cout << Tab "Twoje słowo musi mieć 5 liter" << endl;
        return false;
    }

    return words.find(guess) != words.end();
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

// function that compares user input with answer word
array<int, 5> compareGuess(const vector<string>& answerWordLetters, const vector<string>& guessUpperLetters) {

    // container of leftover characters from initial comparison
    unordered_map<int, string> wordCharacter;

    // array of 0s, 1s, 2s used for marking letters of a user-inputted word
    array<int, 5> letterMarks = {0};

    // 0 - wrong letter, wrong position
    // 1 - correct letter, wrong position
    // 2 - correct letter, correct position
    for (int i = 0; i < 5; i++) {
        if (answerWordLetters[i] == guessUpperLetters[i]) {
            letterMarks[i] = 2;
        } else {
            wordCharacter[i] = answerWordLetters[i];
        }
    }

    for (int i = 0; i < 5; i++) {
        // check if a letter exists at a specific id of unordered_map
        if (wordCharacter.find(i) != wordCharacter.end()) {
            for (int j = 0; j < answerWordLetters.size(); j++) {
                // mark 1s and 0s for the final result
                if (wordCharacter[i] == guessUpperLetters[j] && letterMarks[j] == 0) {
                    letterMarks[j] = 1;
                    break;
                }
            }
        }
    }

    return letterMarks;
}

// function that prints the colored game board
void renderBoard(const vector<vector<string>>& storeWords, const vector<array<int, 5>>& storeMarks) {
    // cycle through stored words
    for (int i = 0; i < storeWords.size(); i++) {
        cout << Reset;
        cout << Tab;
        // cycle through individual stored letters
        for (int j = 0; j < storeWords[i].size(); j++) {
            switch (storeMarks[i][j]) {
                case 1:
                    cout << Start << boldText << End;
                    cout << Start << backgroundColor << elsewhereYellow << End;
                    cout << Start << textColor << letteringWhite << End;
                    cout << storeWords[i][j];
                    break;
                case 2:
                    cout << Start << boldText << End;
                    cout << Start << backgroundColor << correctGreen << End;
                    cout << Start << textColor << letteringWhite << End;
                    cout << storeWords[i][j];
                    break;
                default:
                    cout << Start << boldText << End;
                    cout << Start << backgroundColor << wrongGrey << End;
                    cout << Start << textColor << letteringWhite << End;
                    cout << storeWords[i][j];
                    break;
            }
        }
        cout << endl;
    }
    // reset formatting
    cout << Reset << endl;
}

void printTitle() {
    string logo[10] = {"  +++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ++++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ",
                      "  +++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ++++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ",
                      "  ++oo+++ooo+++oo++  +++++ooooooo+++++  ++++oooooooo+++++  ++++oooooooo++++++  ++++ooo++++++++++  +++++ooooooo+++++  ",
                      "  ++oo++ooooo++oo++  ++++ooo+++ooo++++  ++++ooo+++ooo++++  ++++ooo+++oooo++++  ++++ooo++++++++++  +++++oo++++++++++  ",
                      "  +++oo+oo+oo+oo+++  ++++oo+++++oo++++  ++++oooooooo+++++  ++++ooo++++ooo++++  ++++ooo++++++++++  +++++oooooo++++++  ",
                      "  +++ooooo+ooooo+++  ++++ooo+++ooo++++  ++++ooo+++ooo++++  ++++ooo+++oooo++++  ++++ooo++++++++++  +++++oo++++++++++  ",
                      "  ++++ooo+++ooo++++  +++++ooooooo+++++  ++++ooo+++ooo++++  ++++oooooooo++++++  ++++oooooooo+++++  +++++ooooooo+++++  ",
                      "  +++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ++++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ",
                      "  +++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ++++++++++++++++++  +++++++++++++++++  +++++++++++++++++  ",};

	for(int i = 0; i < 9; i++)
	{
		string logo_line = logo[i];
		for(int j = 0; j < logo_line.size(); j++)
		{
			if(logo_line[j] == '+')
			cout << Start << backgroundColor << correctGreen << End << Start << textColor << correctGreen << End << logo_line[j] << Reset;
			else if(logo_line[j] == 'o')
			cout << Start << backgroundColor << letteringWhite << End << Start << textColor << letteringWhite << End << logo_line[j] << Reset;
			else cout << " ";
		}
		cout << endl;
	}
    cout << "                                                                                                      ";
    cout << Start << backgroundColor << patrioticRed << End  << Start << textColor << letteringWhite << End  << Start << boldText << End << "WERSJA POLSKA" << Reset << endl;
}

int main() {
    // fix Polish character encoding in console (output & input)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // load dictionaries
    vector<string> dictionaryVector = loadWordsFromFile("../dictionary.txt");
    vector<string> answerWordList = loadWordsFromFile("../answer_word_list.txt");

    // convert dictionary to unordered_set
    unordered_set<string> dictionary(dictionaryVector.begin(), dictionaryVector.end());

    if (dictionary.empty() || answerWordList.empty()) {
        cerr << "Błąd: Nie udało się wczytać słowników!" << endl;
        return 1;
    }

    printTitle();

    cout << Tab "Odgadnij 5-literowe słowo w 6 próbach."<< endl;
    cout << Tab "Po każdej próbie otrzymasz wskazówkę:" << endl;
    cout << Tab  "- litera na właściwym miejscu: kolor zielony"<< endl;
    cout << Tab  "- litera występuje w słowie, ale jest w złym miejscu: kolor żółty"<< endl;
    cout << Tab "- litery nie ma w słowie: kolor szary"<< endl;
    cout << Tab "[1] Start" << endl;
    cout << Tab "[2] Wyjście z gry" << endl;

    string option;

    while (option != "1" && option != "2") {
        cout << Tab "Wpisz liczbę: ";
        cin >> option;

        if (option == "1") {
            system("CLS");
            break;
        }
        if (option == "2") {
            return 1;
        }
            cout << Tab "Podano niepoprawną opcję" << endl;
    }

    bool replay = true;

   while (replay==true) {
       // get random word from answerWordList
       string answerWord = getRandomWord(answerWordList);
       string guessWord;

       int guessCounter = 0;
       int markCounter = 0;

       vector<vector<string>> storeWords;
       vector<array<int, 5>> storeMarks;
       vector<string> answerWordLetters = wordLetters(answerWord);

       // main game loop
       replay = false;
       while (guessCounter < 6) {
           cout << Tab "Podaj słowo: ";
           cin >> guessWord;
           string guessUpper = toUpperCasePolish(guessWord);

           while(validateGuessWord(guessUpper, dictionary) != true)
           {
               cout << Tab "Niepoprawne słowo! Podaj nowe: ";
               cin >> guessWord;
               guessUpper = toUpperCasePolish(guessWord);
           }

           vector<string> guessUpperLetters = wordLetters(guessUpper);
           array<int, 5> letterMarks = compareGuess(answerWordLetters, guessUpperLetters);
           system("cls");

           // store words and marking for their letters
           storeWords.push_back(guessUpperLetters);
           storeMarks.push_back(letterMarks);

           renderBoard(storeWords, storeMarks);

           for (int i = 0; i < 5; i++) {
               if (letterMarks[i] == 2)
                   markCounter++;
           }

           if (markCounter == 5) {
               cout << Tab "Poprawna odpowiedź!" << endl;
               break;
           }

           markCounter = 0;

           guessCounter++;
           cout << Tab "Zła odpowiedź! Pozostało " << 6 - guessCounter << " prób."<< endl;
       }

       system("CLS");
       printTitle();
       cout << Tab "Koniec gry!"<< endl << endl;
       renderBoard(storeWords, storeMarks);

       if (guessCounter == 6) {
           cout << Tab "Przegrałes!" << endl;
       }
       else {
           cout << Tab "Wygrałeś!" << endl;
           cout << Tab "Udało ci się zgadnąć za " << guessCounter+1 << ". razem!" << endl;
       }
       cout << Tab "[1] Zagraj ponownie" << endl;
       cout << Tab "[2] Wyjście z gry" << endl;
       option="";

       while (option != "1" && option != "2") {
           cout << Tab "Wpisz liczbę: ";
           cin >> option;

           if (option == "1") {
               system("CLS");
               replay = true;
               break;
           }
           if (option == "2") {
               return 1;
           }
           cout << Tab "Podano niepoprawną opcję" << endl;
       }
   }

    return 0;
}