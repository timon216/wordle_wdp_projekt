#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <unordered_set>

#include "wordle_functions.hpp"

// local helpers
namespace {

// class that allows us to "see" console output
class StreamCapture {
public:
    explicit StreamCapture(std::ostream& stream)
        : stream_(stream), oldBuf_(stream.rdbuf(capture_.rdbuf())) {}

    ~StreamCapture() {
        stream_.rdbuf(oldBuf_);
    }

    std::string str() const {
        return capture_.str();
    }

private:
    std::ostream& stream_;
    std::streambuf* oldBuf_;
    std::ostringstream capture_;
};

// function that creates temp file with given text
std::string createTempFileWithContent(const std::string& content) {
    // Get the temp directory
    const char* tempDir = std::getenv("TEMP");
    if (!tempDir) {
        tempDir = std::getenv("TMP");
    }
    if (!tempDir) {
        tempDir = ".";  // fallback to current directory
    }

    // Create a unique temp filename
    static int counter = 0;
    std::string tempPath = std::string(tempDir) + "/wordle_test_" + std::to_string(counter++) + ".txt";

    std::ofstream out(tempPath, std::ios::binary);
    out << content;
    out.close();

    return tempPath;
}

}

TEST(LoadWordsFromFileTest, HandlesMissingFiles) {
    StreamCapture errorCapture(std::cerr);
    const std::vector<std::string> words = loadWordsFromFile("__brak_pliku_testowego__.txt");

    EXPECT_TRUE(words.empty());
    EXPECT_NE(errorCapture.str().find("Błąd: Nie można otworzyć pliku"), std::string::npos);
}

TEST(LoadWordsFromFileTest, HandlesEmptyLines) {
    const std::string filePath = createTempFileWithContent("KALKA\n\nDOMEK\n\n");
    const std::vector<std::string> words = loadWordsFromFile(filePath);

    ASSERT_EQ(words.size(), 2u);
    EXPECT_EQ(words[0], "KALKA");
    EXPECT_EQ(words[1], "DOMEK");

    std::remove(filePath.c_str());
}

TEST(LoadWordsFromFileTest, PreservesWordsWithPolishLetters) {
    const std::string filePath = createTempFileWithContent("ŻÓŁĆ\nŁANIA\n");
    const std::vector<std::string> words = loadWordsFromFile(filePath);

    ASSERT_EQ(words.size(), 2u);
    EXPECT_EQ(words[0], "ŻÓŁĆ");
    EXPECT_EQ(words[1], "ŁANIA");

    std::remove(filePath.c_str());
}

TEST(GetRandomWordTest, HandlesEmptyFile) {
    EXPECT_TRUE(getRandomWord({}).empty());
}

TEST(GetRandomWordTest, HandlesSingleElementInput) {
    EXPECT_EQ(getRandomWord({"DOMEK"}), "DOMEK");
}

TEST(GetRandomWordTest, ReturnsWordFromInput) {
    const std::vector<std::string> words = {"ANODA", "BIAŁY", "DOMEK", "KALKA"};
    const std::unordered_set<std::string> allowed(words.begin(), words.end());

    for (int i = 0; i < words.size(); ++i) {
        const std::string result = getRandomWord(words);
        // checks if the word is in the unordered set
        EXPECT_EQ(allowed.count(result), 1u);
    }
}

TEST(CountLettersTest, HandlesAsciiAndUtf8) {
    EXPECT_EQ(countLetters(""), 0u);
    EXPECT_EQ(countLetters("KOT"), 3u);
    EXPECT_EQ(countLetters("ŁÓDŹ"), 4u);
    EXPECT_EQ(countLetters("AĄB"), 3u);
}

TEST(ToUpperCasePolishTest, HandlesAsciiAndPolishLetters) {
    EXPECT_EQ(toUpperCasePolish("abcde"), "ABCDE");
    EXPECT_EQ(toUpperCasePolish("zażółć"), "ZAŻÓŁĆ");
    EXPECT_EQ(toUpperCasePolish("aLndĄŻ-ź"), "ALNDĄŻ-Ź");
    EXPECT_EQ(toUpperCasePolish("ŻÓŁĆ!"), "ŻÓŁĆ!");
}

TEST(ValidateGuessWordTest, HandlesCorrectInput) {
    const std::unordered_set<std::string> dictionary = {"KOTEK", "ŁANIA", "DOMEK"};
    EXPECT_TRUE(validateGuessWord("KOTEK", dictionary));
}

TEST(ValidateGuessWordTest, HandlesIncorrectInput) {
    const std::unordered_set<std::string> dictionary = {"KOTEK", "ŁANIA"};
    EXPECT_FALSE(validateGuessWord("DOMEK", dictionary));
}

TEST(ValidateGuessWordTest, RejectsTooShortInput) {
    const std::unordered_set<std::string> dictionary = {"KOTEK", "ŁANIA"};

    StreamCapture outputCapture(std::cout);
    EXPECT_FALSE(validateGuessWord("KOT", dictionary));
    EXPECT_NE(outputCapture.str().find("Twoje słowo musi mieć 5 liter"), std::string::npos);
}

TEST(ValidateGuessWordTest, RejectsTooLongInput) {
    const std::unordered_set<std::string> dictionary = {"KOTEK", "ŁANIA"};

    StreamCapture outputCapture(std::cout);
    EXPECT_FALSE(validateGuessWord("DOMECZEK", dictionary));
    EXPECT_NE(outputCapture.str().find("Twoje słowo musi mieć 5 liter"), std::string::npos);
}

TEST(splitWordTest, SplitsWordIntoLetters) {
    EXPECT_EQ(splitWord("KOTEK"), (std::vector<std::string>{"K", "O", "T", "E", "K"}));
    EXPECT_EQ(splitWord("ŁANIA"), (std::vector<std::string>{"Ł", "A", "N", "I", "A"}));
    EXPECT_TRUE(splitWord("").empty());
}

TEST(CompareGuessTest, MarksFullMatch) {
    const std::vector<std::string> answer = {"K", "O", "T", "E", "K"};
    const std::vector<std::string> guess = {"K", "O", "T", "E", "K"};

    EXPECT_EQ(compareGuess(answer, guess), (std::array<int, 5>{2, 2, 2, 2, 2}));
}

TEST(CompareGuessTest, MarksNoMatches) {
    const std::vector<std::string> answer = {"A", "B", "C", "D", "E"};
    const std::vector<std::string> guess = {"F", "G", "H", "I", "J"};

    EXPECT_EQ(compareGuess(answer, guess), (std::array<int, 5>{0, 0, 0, 0, 0}));
}

TEST(CompareGuessTest, MarksLettersInWrongPositions) {
    const std::vector<std::string> answer = {"A", "B", "C", "D", "E"};
    const std::vector<std::string> guess = {"B", "A", "E", "C", "D"};

    EXPECT_EQ(compareGuess(answer, guess), (std::array<int, 5>{1, 1, 1, 1, 1}));
}

TEST(CompareGuessTest, HandlesDuplicateLetters) {
    const std::vector<std::string> answer = {"A", "A", "B", "B", "C"};
    const std::vector<std::string> guess = {"A", "B", "A", "B", "C"};

    EXPECT_EQ(compareGuess(answer, guess), (std::array<int, 5>{2, 1, 1, 2, 2}));
}

TEST(RenderBoardTest, PrintsGreyBackground) {
    const std::vector<std::vector<std::string>> words = {{"A", "B", "C", "D", "E"}};
    const std::vector<std::array<int, 5>> marks = {{1, 1, 0, 2, 0}};

    StreamCapture outputCapture(std::cout);
    renderBoard(words, marks);

    const std::string output = outputCapture.str();

    EXPECT_NE(output.find("48;2;80;80;80m"), std::string::npos); // grey
    EXPECT_NE(output.find("48;2;212;157;4m"), std::string::npos); // yellow
    EXPECT_NE(output.find("48;2;8;199;30m"), std::string::npos); // green
}