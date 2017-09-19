#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define MASTERMIND_CODE_SIZE 4
#define MAX_CODE_VALUE 5
#define MAX_GUESSES 10


/* codeElement
*
* Stores an element in such a way that that the original index can be found if vectors are resized during checkIncorrect()
*/
struct codeElement
{
	int index;
	int value;
};

/* findInSequence
*
* Finds the first instance of value in the given sequence
*
* @param sequence std::vector<codeElement> *
* @param value int
*/
int findInSequence(std::vector<codeElement> * sequence, int value)
{
	for (auto iterator = sequence->begin(); iterator < sequence->end(); iterator++)
	{
		if (iterator->value == value)
		{
			return (int)(iterator - sequence->begin());
		}
	}

	return -1;
}

class response
{
private:
	int correct;   // Numbers in the correct place
	int incorrect; // Numbers that would be correct but are in the wrong place

public:

	/* constructor
	*
	* Creates a response object with provided values
	*
	* @param c int
	* @param i int
	*/
	response(int c, int i) : correct(c), incorrect(i) {}

	/* setCorrect
	*
	* Sets the value of the correct field
	*
	* @param newValue int
	*/
	void setCorrect(int newValue)
	{
		correct = newValue;
	}

	/* setIncorrect
	*
	* Sets the value of the incorrect field
	*
	* @param newValue int
	*/
	void setIncorrect(int newValue)
	{
		incorrect = newValue;
	}

	/* getCorrect
	*
	* Gets the value of the correct field
	*/
	int getCorrect()
	{
		return correct;
	}

	/* getIncorrect
	*
	* Gets the value of the incorrect field
	*/
	int getIncorrect()
	{
		return incorrect;
	}

	/* compare
	*
	* Compares two responses and returns true if the are equivalent
	*
	* @param response response
	*/
	bool compare(response response)
	{
		if (correct == response.correct && incorrect == response.incorrect)
			return true;

		return false;
	}

	/* print
	*
	* Prints a response object
	*/
	void print()
	{
		std::cout << "Response - Correct (Exact Match): " << correct << ", Incorrect (Wrong Place): " << incorrect << std::endl;
	}
};

class code
{
private:
	std::vector<int> sequence;
	
public:
	/* checkCorrect
	*
	* Compares the code sequence to the correct code and returs the number of numbers in the correct location
	*
	* @param correctCode std::vector<int>
	*/

	/* code
	*
	* Constructor to initialize a random code
	*/
	code()
	{
		initialize();
	}

	/* code
	*
	* Constructor to create a code object from a predefined vector
	*
	* @param newSequence std::vector<int>
	*/
	code(std::vector<int> newSequence)
	{
		// Manually create a new code sequence
		try
		{
			if (newSequence.size() != MASTERMIND_CODE_SIZE)
				throw "ERROR: Vector was of incorrect length!";

			sequence = newSequence;
		}
		catch (std::string error)
		{
			std::cout << error << std::endl;
		}

	}
	int checkCorrect(code correctCode)
	{
		// Find the number of corect values
		int correct = 0;
		for (int index = 0; index < (int)sequence.size(); index++)
		{
			if (sequence[index] == correctCode.sequence[index])
				correct++;
		}

		return correct;
	}

	/* checkIncorrect
	*
	* Compares the code sequence to the correct code and returns a response object with thier correlation
	*
	* @param correctCode std::vector<int>
	*/
	int checkIncorrect(code correctCode)
	{
		int incorrect = 0;

		// Create a copy of the correct sequence so we can modify it
		std::vector<int> sequenceGuess(sequence);

		// Rmove correct values from both correct and guess sequences
		int index = 0;
		while (index < (int)sequenceGuess.size())
		{
			if (sequenceGuess[index] == correctCode.sequence[index])
			{
				// Found a correct value, remove matching (correct) values
				sequenceGuess.erase(sequenceGuess.begin() + index);
				correctCode.sequence.erase(correctCode.sequence.begin() + index);
			}

			// If an value was removed, iterating the index will skip the next value in the vector
			else
				index++;
		}

		// Look for incorrect (wrong place) values in the remainder of the sequency
		for (int index = 0; index < (int)sequenceGuess.size(); index++)
		{
			// Look for each sequence element in the correct code
			auto iterator = std::find(correctCode.sequence.begin(), correctCode.sequence.end(), sequenceGuess[index]);
			if (iterator != correctCode.sequence.end())
			{
				// Value was found (indicating a wrong place value)
				incorrect++;

				// Remove the element from the sequence so it is not found again
				correctCode.sequence.erase(iterator);
			}
		}

		// Return number of incorrect values
		return incorrect;
	}

	/* initialize
	*
	* INitializes value to be a random sequence of MASTERMIND_CODE_SIZE integers in the range of 0 : MAX_CODE_VALUE
	*/
	void initialize()
	{
		// Set value to be random integers from 0 - MAX_CODE_VALUE
		srand((int)time(NULL));
		for (int index = 0; index < MASTERMIND_CODE_SIZE; index++)
		{
			sequence.push_back(rand() % (MAX_CODE_VALUE + 1));
		}
	}

	/* print
	*
	* Prints a code object
	*/
	void print()
	{
		std::cout << "Code - {";
		for (int index = 0; index < (int)sequence.size(); index++)
		{
			std::cout << sequence[index];
			if (index < (int)sequence.size() - 1)
				std::cout << ", ";
		}
		std::cout << "}" << std::endl;
	}
};

class mastermind
{
private:
	code guessSequence;
	bool gameEnd = false;

public:

	/*
	* Call the print the secret code
	*/
	void printSecretCode(code secrectcode)
	{
		secrectcode.print();
	}

	/*
	* Read the a guess from the keyboard and stored in a code object
	*/
	void humanGuess()
	{	
		std::vector<int> userGuess(MASTERMIND_CODE_SIZE);

		for (int index = 0; index < MASTERMIND_CODE_SIZE; index++)
		{
			int input = 0;
			do
			{
				// Display error for bad input
				if (std::cin.fail() || input < 0 || input > MAX_CODE_VALUE)
				{
					std::cout << "ERROR, invalid input" << std::endl;

					// Clear cin buffer
					std::cin.clear();
					std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');
				}

				// Ask user to enter the guess number;
				std::cout << "Please enter code digit " << index + 1 << " in the range of 0 to " << MAX_CODE_VALUE << ": ";
				std::cin >> input;

			} while (std::cin.fail() || input < 0 || input > MAX_CODE_VALUE);

			userGuess[index] = input;
		}

		// Initialize the guessSequence to be the users guess
		guessSequence = code(userGuess);

		// Print out the guess code user entered
		guessSequence.print();
	}

	/*
	*
	* @params secretcode std::vector<int>, guessCode std::vector<int>
	*/
	void getResponse(code secretCode, code guessCode)
	{
		int correct=secretCode.checkCorrect(guessCode);
		int incorrect = secretCode.checkIncorrect(guessCode);

		// Create a response object with the values correct/incorrect values
		response response(correct, incorrect);
		isSolved(response);
	}

	/*
	* Return a response base on boolean of response 
	* True - codebreaker won the game
	* False - game continue and print the result
	* @params response checkRepsonse 
	*/
	void isSolved(response checkRepsonse)
	{
		if (checkRepsonse.getCorrect() == MASTERMIND_CODE_SIZE)
		{
			gameEnd = true;
			std::cout << std::endl << "The board has been solved. You, the codebreaker have won the game!" << std::endl;
		}
		else
		{
			checkRepsonse.print();
		}
		
	}

	/*
	* start the game, and inititalize the secert code, and user only have 10 tries to win the game
	*/
	void playGame()
	{
		int index = 0;
		code secretCode;
		std::cout << "A secret code had been generated" << std::endl;

		// ten tries for the user
		do {
			index++;
			std::cout << "Guess Number " << index << std::endl;
			humanGuess();
			getResponse(secretCode, guessSequence);
			if (gameEnd) {
				break;
			}
			std::cout << std::endl;
		} while (index < MAX_GUESSES);

		// If secrete code is not guess correct in 10 tries, this message will appeare.
		if (!gameEnd) 
		{
			std::cout << "Secret ";
			printSecretCode(secretCode);
			std::cout << std::endl << "You just lost game. CodeMaker win!!" << std::endl << std::endl;
		}
	}
};
