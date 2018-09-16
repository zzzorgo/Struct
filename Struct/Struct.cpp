// Struct.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Struct.h"
#include <vector>
#include <algorithm>
#include <fstream> 
#include <iterator>

using namespace std;

class LetterProbability {
	public: 
	char letter;
	int letterCount;
	string prefix;
	LetterProbability() : letterCount(0), prefix("") { }
};

ostream &operator<<(std::ostream &os, LetterProbability const &lp) { 
    return os << lp.letter << " " << lp.letterCount << endl;
}

class TreeNode {
public:
	int priority;
	string prefix;
	LetterProbability initialNode;
	TreeNode *leftNode;
	TreeNode *rightNode;

	TreeNode(LetterProbability initialNodeL) :
		initialNode(initialNodeL),
		priority(initialNodeL.letterCount),
		leftNode(nullptr),
		rightNode(nullptr),
		prefix("") { }

	TreeNode() : priority(0),
				 prefix("") { }

	void startWalker() {
		leftTreePrefix(this);
		rightTreePrefix(this);
	}

	vector<LetterProbability> startWalkerGatherPrefix() {
		vector<LetterProbability> letterDictionary;

		vector<LetterProbability> leftLetterDictionary = leftTreePrefixGather(this);
		letterDictionary.insert(letterDictionary.end(), leftLetterDictionary.begin(), leftLetterDictionary.end());
		vector<LetterProbability> rightLetterDictionary = rightTreePrefixGather(this);
		letterDictionary.insert(letterDictionary.end(), rightLetterDictionary.begin(), rightLetterDictionary.end());

		return letterDictionary;
	}

private:
	void leftTreePrefix(TreeNode *node) {
		TreeNode *leftNode = node->leftNode;
		if (!leftNode) {
			return;
		}
		leftNode->prefix = node->prefix + '0';
		leftTreePrefix(leftNode);
		rightTreePrefix(leftNode);
	}

	void rightTreePrefix(TreeNode *node) {
		TreeNode *rightNode = node->rightNode;
		if (!rightNode) {
			return;
		}
		rightNode->prefix = node->prefix + '1';
		leftTreePrefix(rightNode);
		rightTreePrefix(rightNode);
	}

	vector<LetterProbability> leftTreePrefixGather(TreeNode *node) {
		vector<LetterProbability> letterDictionary;
		TreeNode *leftNode = node->leftNode;
		if (!leftNode) {
			LetterProbability lp = LetterProbability();
			lp.letter = node->initialNode.letter;
			lp.prefix = node->prefix;
			letterDictionary.push_back(lp);
			return letterDictionary;
		}

		vector<LetterProbability> leftLetterDictionary = leftTreePrefixGather(leftNode->leftNode);
		letterDictionary.insert(letterDictionary.end(), leftLetterDictionary.begin(), leftLetterDictionary.end());

		vector<LetterProbability> rightLetterDictionary = rightTreePrefixGather(leftNode->leftNode);
		letterDictionary.insert(letterDictionary.end(), rightLetterDictionary.begin(), rightLetterDictionary.end());

		return letterDictionary;
	}

	vector<LetterProbability> rightTreePrefixGather(TreeNode *node) {
		vector<LetterProbability> letterDictionary;
		TreeNode *rightNode = node->rightNode;
		if (!rightNode) {
			LetterProbability lp = LetterProbability();
			lp.letter = node->initialNode.letter;
			lp.prefix = node->prefix;

			letterDictionary.push_back(lp);
			return letterDictionary;
		}

		vector<LetterProbability> leftLetterDictionary = leftTreePrefixGather(rightNode);
		letterDictionary.insert(letterDictionary.end(), leftLetterDictionary.begin(), leftLetterDictionary.end());
		vector<LetterProbability> rightLetterDictionary = rightTreePrefixGather(rightNode);
		letterDictionary.insert(letterDictionary.end(), rightLetterDictionary.begin(), rightLetterDictionary.end());
		return letterDictionary;
	}
};

int main()
{
	ifstream charFile("haffman.txt");
	if (!charFile) {
		return -1;
	}
	vector<char> text;
	charFile >> noskipws;
	copy(istream_iterator<char>(charFile), istream_iterator<char>(),
		std::back_inserter(text));
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

	TreeNode *topNode = treeVector[0];
	topNode->startWalker();
	vector<LetterProbability> lp = topNode->startWalkerGatherPrefix();
	return 0;
}
