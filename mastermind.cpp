#include "mastermind.h"
#include <Windows.h>

int main()
{
	// Testing for comparison operations (Maybe this will be useful while creating the main program loop)
	for (int i = 0; i < 10; i++)
	{
		code newCode, guess;
		newCode.initialize();
		Sleep(1000);
		guess.initialize();

		std::cout << "Correct Code: ";
		newCode.print();
		std::cout << "Guess:        ";
		guess.print();

		int correct = newCode.checkCorrect(guess);
		response response = newCode.checkIncorrect(guess);

		std::cout << "Is Correct?: " << correct << std::endl;
		response.print();

		system("pause");
	}
}