// Struct.cpp : Defines the entry point for the application.
//

#include "Struct.h"
#include <vector>
#include <algorithm>
#include <fstream> 
#include <iterator>

using namespace std;

class LetterProbability {
	public: 
	char letter;
	double probability;
	int letterCount;

	LetterProbability() : probability(0), letterCount(0) { }
};

ostream &operator<<(std::ostream &os, LetterProbability const &lp) { 
    return os << lp.letter << " " << lp.letterCount << " " << lp.probability << endl;
}


class TreeNode {
public:
	int priority;
	LetterProbability initialNode;
	TreeNode *leftNode;
	TreeNode *rightNode;

	TreeNode(LetterProbability initialNodeL) :
		initialNode(initialNodeL),
		priority(initialNodeL.letterCount),
		leftNode(nullptr),
		rightNode(nullptr) { }

	TreeNode() : priority(0) { }
};

int main()
{
	ifstream charFile("haffman.txt");
	if (!charFile) {
		return -1;
	}
	istream_iterator<char> start(charFile), end;
	vector<char> text(start, end);
    copy(text.begin(), text.end(),
		ostream_iterator<char>(cout));
	cout << endl;


	vector<LetterProbability> letterProbability;

	for (int i = 0; i < text.size(); i++) {
		char c = text[i];
		auto foundedChar = find_if(letterProbability.begin(), letterProbability.end(), [&c](const LetterProbability& obj) {return obj.letter == c;});
		

		int index = -1;
		if (foundedChar != letterProbability.end()) {
			index = distance(letterProbability.begin(), foundedChar);
		}
		if (index < 0) {
			LetterProbability lp = LetterProbability();
			lp.letter = c;
			lp.letterCount++;
			letterProbability.push_back(lp);
		} else {
			letterProbability[index].letterCount++;
		}
	}


	sort(letterProbability.begin(), letterProbability.end(), [](const LetterProbability& a, const LetterProbability& b) {
		return a.letterCount < b.letterCount;
	});

	for (auto lp : letterProbability) {
		lp.probability = (double)lp.letterCount / text.size();
		cout << lp << endl;
	}

	vector<TreeNode*>treeVector;

	for (LetterProbability lp : letterProbability) {
		treeVector.push_back(new TreeNode(lp));
	}

	for (int i = 0; i < letterProbability.size() - 1; i++) {
		TreeNode* firstNode = treeVector[0];
		TreeNode* secondNode = treeVector[1];

		TreeNode* newNode = new TreeNode();
		(*newNode).priority = (*firstNode).priority + (*secondNode).priority;

		(*newNode).leftNode = firstNode;
		(*newNode).rightNode = secondNode;

		treeVector.erase(treeVector.begin());
		treeVector.erase(treeVector.begin());

		treeVector.push_back(newNode);
		sort(treeVector.begin(), treeVector.end(), [](const TreeNode *a, const TreeNode *b) {
			return a->priority < b->priority;
		});
	}

	return 0;
}
