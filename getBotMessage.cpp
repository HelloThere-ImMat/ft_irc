#include <iostream>
#include <string>

int getRandNb(int range)
{
	int rNb;

	srand(time(NULL));
	rNb = rand() % range;
	return (rNb);
}

void printBotMessage()
{


}

void bot(std::string message, std::string clientName)
{
	std::string loadingStrs[] = {"Loading...", "..", "...", "hhmmmmmm..", "Let me think..."};



	std::string::const_iterator lastChar= message.cend() - 1;
	std::cout << "LOG = Last char is : " << *lastChar << std::endl;
	if (*lastChar == '?')
		std::cout << "BOT = I don't answer stupid questions" << std::endl;
	else
	{
		int loadingPrintsNb =  getRandNb(4);
		std::cout << "LOG = loading print nb : " << loadingPrintsNb << std::endl;
		for (int i = 0; i < loadingPrintsNb; i++)
		{
			int loadingIndex = getRandNb(loadingStrs->size());
			std::cout << "LOG = loading Index: " << loadingIndex << std::endl;
			std::cout << "BOT = " << loadingStrs[loadingIndex] << std::endl;
			i++;
		}
		std::cout << "BOT = Fuck you" << std::endl;
	}

	//if ()
}


int main()
{
	bot("HI I AM MAT", "mat");
	bot("how are you ?", "mat");
	return (0);
}