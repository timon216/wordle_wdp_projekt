#pragma once

#include <array>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<std::string> loadWordsFromFile(const std::string& filename);
std::string getRandomWord(const std::vector<std::string>& words);
size_t countLetters(const std::string& word);
std::string toUpperCasePolish(const std::string &word);
bool validateGuessWord(const std::string& guess, const std::unordered_set<std::string>& words);
std::vector<std::string> splitWord(const std::string& word);
std::array<int, 5> compareGuess(const std::vector<std::string>& answerWordLetters, const std::vector<std::string>& guessUpperLetters);
void renderBoard(const std::vector<std::vector<std::string>>& storeWords, const std::vector<std::array<int, 5>>& storeMarks);

