#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>

class Hangman {
private:
    std::vector<std::string> wordList = {
        "programming", "hangman", "computer", "keyboard",
        "developer", "algorithm", "function", "variable",
        "language", "software"
    };

    std::string secretWord;
    std::string guessedWord;
    std::vector<char> guessedLetters;
    int wrongGuesses;
    const int maxWrongGuesses = 6;

    int hintsUsed;
    const int maxHints = 2;

    const std::string hangmanStages[7] = {
        "\n  +---+\n      |\n      |\n      |\n     ===",
        "\n  +---+\n  O   |\n      |\n      |\n     ===",
        "\n  +---+\n  O   |\n  |   |\n      |\n     ===",
        "\n  +---+\n  O   |\n /|   |\n      |\n     ===",
        "\n  +---+\n  O   |\n /|\\  |\n      |\n     ===",
        "\n  +---+\n  O   |\n /|\\  |\n /    |\n     ===",
        "\n  +---+\n  O   |\n /|\\  |\n / \\  |\n     ==="
    };

public:
    Hangman() : wrongGuesses(0), hintsUsed(0) {
        srand(static_cast<unsigned int>(time(0)));
        chooseWord();
    }

    void chooseWord() {
        int index = rand() % wordList.size();
        secretWord = wordList[index];
        guessedWord = std::string(secretWord.length(), '_');
    }

    void displayState() {
        std::cout << hangmanStages[wrongGuesses] << "\n\n";
        std::cout << "Word: ";
        for (char c : guessedWord) {
            std::cout << c << ' ';
        }
        std::cout << "\n\n";

        std::cout << "Guessed letters: ";
        for (char c : guessedLetters) {
            std::cout << c << ' ';
        }
        std::cout << "\n";

        std::cout << "Wrong guesses left: " << (maxWrongGuesses - wrongGuesses) << "\n";
        std::cout << "Hints left: " << (maxHints - hintsUsed) << " (type 'hint' to use one)\n\n";
    }

    bool alreadyGuessed(char letter) {
        return std::find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end();
    }

    void makeGuess(char letter) {
        guessedLetters.push_back(letter);
        bool found = false;

        for (size_t i = 0; i < secretWord.length(); i++) {
            if (secretWord[i] == letter) {
                guessedWord[i] = letter;
                found = true;
            }
        }

        if (!found) {
            wrongGuesses++;
            std::cout << "Wrong guess!\n";
        } else {
            std::cout << "Good guess!\n";
        }
    }

    void useHint() {
        if (hintsUsed >= maxHints) {
            std::cout << "No hints left!\n\n";
            return;
        }

        // Collect indices of letters not yet revealed
        std::vector<int> hiddenIndices;
        for (size_t i = 0; i < secretWord.length(); i++) {
            if (guessedWord[i] == '_') {
                hiddenIndices.push_back(static_cast<int>(i));
            }
        }

        if (hiddenIndices.empty()) {
            std::cout << "No more letters to reveal!\n\n";
            return;
        }

        int randomIndex = hiddenIndices[rand() % hiddenIndices.size()];
        char revealedLetter = secretWord[randomIndex];

        guessedWord[randomIndex] = revealedLetter;
        if (!alreadyGuessed(revealedLetter)) {
            guessedLetters.push_back(revealedLetter);
        }

        hintsUsed++;
        wrongGuesses++; // hint costs a wrong guess (penalty)

        std::cout << "Hint used! Revealed letter: '" << revealedLetter << "'\n";
        std::cout << "(This counted as a wrong guess.)\n\n";
    }

    bool isWordGuessed() {
        return guessedWord == secretWord;
    }

    bool isGameOver() {
        return wrongGuesses >= maxWrongGuesses;
    }

    void play() {
        std::cout << "=== Welcome to Hangman ===\n";
        std::cout << "Try to guess the hidden word one letter at a time.\n";
        std::cout << "Type 'hint' at any time to reveal a letter (costs a wrong guess).\n";

        while (!isGameOver() && !isWordGuessed()) {
            displayState();

            std::cout << "Enter a letter (or 'hint'): ";
            std::string input;
            std::cin >> input;

            // Convert input to lowercase for comparison
            std::string lowerInput = input;
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

            if (lowerInput == "hint") {
                useHint();
                continue;
            }

            if (input.length() != 1 || !isalpha(input[0])) {
                std::cout << "Please enter a single valid letter or 'hint'.\n\n";
                continue;
            }

            char letter = tolower(input[0]);

            if (alreadyGuessed(letter)) {
                std::cout << "You already guessed that letter.\n\n";
                continue;
            }

            makeGuess(letter);
            std::cout << "\n";
        }

        displayState();

        if (isWordGuessed()) {
            std::cout << "Congratulations! You guessed the word: " << secretWord << "\n";
        } else {
            std::cout << "Game over! The word was: " << secretWord << "\n";
        }
    }
};

int main() {
    char playAgain;

    do {
        Hangman game;
        game.play();

        std::cout << "\nPlay again? (y/n): ";
        std::cin >> playAgain;
        std::cout << "\n";

    } while (tolower(playAgain) == 'y');

    std::cout << "Thanks for playing Hangman!\n";
    return 0;
}
