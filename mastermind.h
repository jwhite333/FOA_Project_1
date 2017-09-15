#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define MASTERMIND_CODE_SIZE 4
#define MAX_CODE_VALUE 5


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
	std::vector<codeElement> sequence;

public:
	/* checkCorrect
	*
	* Compares the code sequence to the correct code and returs true if they are equivalient
	*
	* @param correctCode std::vector<int>
	*/
	bool checkCorrect(code correctCode)
	{
		// Make sure no out of range errors occur
		if ((int)sequence.size() != correctCode.sequence.size())
			return false;

		for (int index = 0; index < (int)sequence.size(); index++)
		{
			if (sequence[index].value != correctCode.sequence[index].value)
				return false;
		}

		return true;
	}

	/* checkIncorrect
	*
	* Compares the code sequence to the correct code and returns a response object with thier correlation
	*
	* @param correctCode std::vector<int>
	*/
	response checkIncorrect(code correctCode)
	{
		int correct = 0, incorrect = 0;

		std::vector<codeElement> sequenceCopy(sequence);

		// Look for correct values
		int index = 0;
		while (index < (int)sequenceCopy.size())
		{
			if (sequenceCopy[index].value == correctCode.sequence[index].value)
			{
				// Found a correct value
				correct++;

				// Remove matching (correct) values from code and sequence
				sequenceCopy.erase(sequenceCopy.begin() + index);
				correctCode.sequence.erase(correctCode.sequence.begin() + index);
			}

			// If an value was removed, iterating the index will skip the next value in the vector
			else
				index++;
		}

		// Look for incorrect (wrong place) values in the remainder of the sequency
		for (int index = 0; index < (int)sequenceCopy.size(); index++)
		{
			// Look for each sequence element in the correct code
			int position = findInSequence(&correctCode.sequence, sequenceCopy[index].value);

			// Value was not found in the correct code
			if (position == -1)
				continue;

			// Value was found (indicating a wrong place value)
			incorrect++;

			// Remove the element from the sequence so it is not found again
			correctCode.sequence.erase(correctCode.sequence.begin() + position);
		}

		// Return a response object
		return response(correct, incorrect);
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
			codeElement element;
			element.value = rand() % (MAX_CODE_VALUE + 1);
			element.index = index;
			sequence.push_back(element);
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
			std::cout << sequence[index].value;
			if (index < (int)sequence.size() - 1)
				std::cout << ", ";
		}
		std::cout << "}" << std::endl;
	}
};


