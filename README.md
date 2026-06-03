# WORDLE

[PL below]

## A console version of the word game Wordle with support for polish characters, written in C++.

The gameplay consists of guessing five-letter words in six guesses. After each attempt the player receives feedback: correct letters in correct positions are marked with green, correct letters in incorrect positions are marked with yellow, while letters not present in the word are marked with grey. 

## Downloading the program

1.	Downloading the files from GitHub
2.	Compiling and running the program in CLion:
- Open -> select the project folder
- Reload CMake Project -> Build
- select configuration WORDLE -> Run
4.	Running the tests in CLion:
Run - > Edit Configurations -> change target to WORDLE_TEST -> Run

Warning: file paths of the dictionaries included in the code are written as `../dictionary.txt` and `../answer_word_list.txt`. If any errors occur while reading files, it is advisable to either check whether the files are present in the appropriate folder, or to change their path in the code (for example, by removing `../`).

## Konsolowa wersja gry słownej Wordle obsługująca polskie znaki, napisana w C++

Gra polega na zgadywaniu pięcioliterowych słów w sześciu próbach. Po każdej próbie użytkownik otrzymuje informację zwrotną: prawidłowe litery na odpowiedniej pozycji oznaczone są kolorem zielonym, litery znajdujące się w słowie, ale w innym miejscu - żółtym, a brak liter w słowie - szarym.

## Pobranie programu

1.	Pobranie plików z GitHuba
2.	Kompilacja i uruchomienie w CLion:
- Open -> wskaż folder projektu
- Reload CMake Project -> Build
- wybierz konfigurację WORDLE -> Run
4.	Uruchomienie testów w CLion:
Run -> Edit Configurations -> zmień target na WORDLE_TEST -> Run	

Uwaga: ścieżki do plików słowników w kodzie są ustawione jako `../dictionary.txt` i `../answer_word_list.txt`. W przypadku problemów z wczytywaniem plików należy się upewnić, że pliki są w odpowiednim miejscu lub zmienić ścieżkę do nich (np. usunąć `../`)


