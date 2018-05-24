/* This is the console executable that makes use of the BullCow Class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBUllCowGame class
*/

#include <iostream>  //use <> when using standard stuff.
#include <string>
#include "FBullCowGame.h"

using FText = std::string;
using int32 = int;

 
void PrintIntro();
bool InstructionsNeeded();
void PrintInstructions();
int32 GetDesiredWordLength();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();


FBullCowGame BCGame; // instantiate a new game


// the entry point for our application
int main()
{
	bool bPlayAgain = false;
	do {
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	}
	while (bPlayAgain);

	return 0; // exit the application
}

// introduce the game
void PrintIntro()
{
	// print the welcome message
	std::cout << "\nWelcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "           }   {             ___ " << std::endl;
	std::cout << "           (o o)            (o o)  " << std::endl;
	std::cout << "   /--------\\ /              \\ /--------\\ " << std::endl;
	std::cout << "  / | BULL |0                  0|  COW | \\ " << std::endl;
	std::cout << " *  |------|                    |------|  * " << std::endl;
	std::cout << "    ^      ^                    ^      ^ " << std::endl << std::endl;
	
	// ask the user if they know how to play the game, print instructions if not
	if (InstructionsNeeded()) {
		PrintInstructions();
	}

	// Get the user's desired word length and generate an isogram based on that length
	int32 DesiredWordLength = GetDesiredWordLength();
	BCGame.GenerateHiddenWord(DesiredWordLength);

	std::cout << "\n\nCan you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

bool InstructionsNeeded()
{
	std::cout << "Do you know how to play? (Y/N) ";
	FString Response;
	std::getline(std::cin, Response);
	if (Response[0] == 'N' || Response[0] == 'n') {
		return true;
	}
	else {
		return false;
	}
}

void PrintInstructions()
{
	std::cout << "\nInstructions:\n";
	std::cout << "   An isogram is a word without any repeating letters.\n";
	std::cout << "   Enter a guess for the isogram I am thinking of.\n";
	std::cout << "   I will tell you how many bulls and cows are in your guess.\n";
	std::cout << "   A bull means that a letter in your guess matches the letter and place in my word.\n";
	std::cout << "   A cow means that a letter in your guess matches a letter in my word, but it is not in the same place.\n\n";
	return;
}

int32 GetDesiredWordLength()
{
	std::cout << "How many letters would you like in the isogram? ";
	FString WordLengthInput;
	std::getline(std::cin, WordLengthInput);
	int32 WordLength = std::stoi(WordLengthInput);
	return WordLength;
}

void PlayGame()
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	// loop asking for guesses while the game is not won
	// and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) { 
		FText Guess = GetValidGuess(); 

		// Submit valid guess to the game, and recieve counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ". Enter your guess: ";
		
		std::getline(std::cin, Guess);

		// check status and give feedback on guess
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status) {
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;

		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;

		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;

		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon()) {
		std::cout << "Good job!\n\n";
	}
	else {
		std::cout << "Better luck next time!\n\n";
	}
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again? (Y/N) ";
	FText Response = "";
	std::getline(std::cin, Response);
	std::cout << std::endl;
	return (Response[0] == 'y') || (Response[0] == 'Y');
}
