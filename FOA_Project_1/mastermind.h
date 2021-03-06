#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define MASTERMIND_CODE_SIZE 4
#define MAX_CODE_VALUE 5
#define MAX_GUESSES 10

enum inputError
{
	NONE,
	NOT_A_NUMBER,
	OUT_OF_RANGE
};

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
		catch (std::exception error)
		{
			std::cout << error.what() << std::endl;
		}

	}

	/* checkCorrect
	*
	* Compares the code sequence to the correct code and returs the number of numbers in the correct location
	*
	* @param correctCode std::vector<int>
	*/
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
public:

	/*
	* Call the print the secret code
	*/
	void printSecretCode(code secrectcode)
	{
		secrectcode.print();
	}

	/*
	*
	* Read the a guess from the keyboard and return a code object
	*/
	code humanGuess()
	{
		std::vector<int> userGuess(MASTERMIND_CODE_SIZE);

		for (int index = 0; index < MASTERMIND_CODE_SIZE; index++)
		{
			int input = 0;
			inputError error = NONE;
			do
			{
				try
				{
					// Ask user to enter the guess number;
					std::cout << "Please enter code digit " << index + 1 << " in the range of 0 to " << MAX_CODE_VALUE << ": ";
					std::cin >> input;

					// Catch bad input
					if (std::cin.fail())
						throw inputError::NOT_A_NUMBER;

					// Catch out of range input
					if (input < 0 || input > MAX_CODE_VALUE)
						throw inputError::OUT_OF_RANGE;

					error = inputError::NONE;
				}
				catch (inputError e)
				{
					// Set error flag
					error = e;

					// Display error for bad input
					if (e == inputError::NOT_A_NUMBER)
						std::cout << "ERROR, input not a number" << std::endl;
					if (e == inputError::OUT_OF_RANGE)
						std::cout << "ERROR, input is out of acceptable range" << std::endl;

					// Clear cin buffer
					std::cin.clear();
					std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');
				}
				
			} while (error != inputError::NONE);

			userGuess[index] = input;
		}

		// Initialize the guessSequence to be the users guess
		code guessSequence(userGuess);
		return guessSequence;

	}

	/*
	* 
	* generate a response by passing correct/incorrect value and return the response
	* 
	* @params secretcode std::vector<int>, guessCode std::vector<int>
	*/
	response getResponse(code secretCode, code guessCode)
	{
		int correct = secretCode.checkCorrect(guessCode);
		int incorrect = secretCode.checkIncorrect(guessCode);

		// Create a response object with the values correct/incorrect values
		response response(correct, incorrect);
		return response;
	}

	/*
	*
	* Return a boolean base on response 
	* True - codebreaker won the game
	* False - game continue and print the result
	*
	* @params response checkRepsonse 
	*/
	bool isSolved(response checkRepsonse)
	{
		// Check is the number digit correct equal to the size of secret code
		if (checkRepsonse.getCorrect() == MASTERMIND_CODE_SIZE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/*
	*
	* start the game, and inititalize the secert code, and user only have 10 tries to win the game
	*/
	void playGame()
	{
		int index = 0;
		bool success = false;

		//inititalize secret code
		code secretCode;
		std::cout << "A secret code had been generated, try to guess it!" << std::endl << std::endl;

		// ten tries for the user
		do {
			index++;
			std::cout << "Guess #" << index << std::endl;

			code guessSequence = humanGuess();
		
			std::cout << "Your guess is: ";
			guessSequence.print();

			//get the response
			response responses = getResponse(secretCode, guessSequence);
			isSolved(responses);

			//check is the secretcode is solved or not
			if (isSolved(responses)) {
				std::cout << std::endl << "The board has been solved. You, the codebreaker have won the game!" << std::endl;
				success = true;
				break;
			}
			else {
				responses.print();
			}
			
			std::cout << std::endl;
		} while (index < MAX_GUESSES);

		// If secrete code is not guess correct in 10 tries, this message will appeare and game end.
		if (!success) 
		{
			std::cout << "You were unable to guess the code in " << MAX_GUESSES << " tries!" << std::endl
				<< "The correct Secret Code was: ";
			printSecretCode(secretCode);
		}
	}
};
