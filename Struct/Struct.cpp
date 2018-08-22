// Struct.cpp : Defines the entry point for the application.
//

#include "Struct.h"
#include <vector>

using namespace std;

class LetterProbability {
	public: 
	char letter;
	double probability;
	int letterCount;

	LetterProbability() {
		probability = 0;
		letterCount = 0;
	}
};

ostream &operator<<(std::ostream &os, LetterProbability const &lp) { 
    return os << lp.letter << " " << lp.letterCount << endl;
}

int main()
{
	vector<char> text;
	char c;
	while (cin >> c) {
		text.push_back(c);
	}

	vector<LetterProbability> letterProbability;

	for (int i = 0; i < text.capacity(); i++) {
		char c = text[i];
		auto foundedChar = find_if(letterProbability.begin(), letterProbability.end(), [&c](const LetterProbability& obj) {return obj.letter == c;});

		int index = -1;
		if (foundedChar != letterProbability.end()) {
			index = distance(letterProbability.begin(), foundedChar);
		}
		if (index < 0) {
			LetterProbability lp = LetterProbability();
			lp.letter = text[index];
			lp.letterCount++;
			letterProbability.push_back(lp);
		} else {
			letterProbability[index].letterCount++;
		}
	}

	for (auto lp : letterProbability) {
		cout << lp << endl;
	}

	return 0;
}
