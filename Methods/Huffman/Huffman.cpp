#include <iostream>
#include <list>
#include <string>
#include <map>
using namespace std;

struct element {
	string value = "";
	string number = "2";
	element* root = nullptr;
	unsigned long long amount = 0;
	bool isCalculated = false;
};

void huffman_tree(map<string, element>& tree) {
	string key_1;
	string key_2;
	for (int counter = tree.size() - 1; counter > 0; --counter) {
		unsigned long long min_1 = UINT64_MAX;
		unsigned long long min_2 = UINT64_MAX;
		key_1 = "";
		key_2 = "";
		for (map<string, element>::iterator it = tree.begin(); it != tree.end(); ++it) {
			if (!(it->second.isCalculated)) {
				if (it->second.amount < min_1) {
					min_2 = min_1;
					key_2 = key_1;
					min_1 = it->second.amount;
					key_1 = it->first;
					continue;
				}
				if (it->second.amount < min_2) {
					min_2 = it->second.amount;
					key_2 = it->first;
				}
			}
		}
		string temp = key_1 + " " + key_2;
		tree.insert(pair<string, element>(temp, element()));
		tree[temp].amount = min_1 + min_2;
		tree[temp].value = temp;

		tree[key_1].root = &(tree[temp]);
		tree[key_2].root = &(tree[temp]);
		tree[key_1].number = "0";
		tree[key_2].number = "1";
		tree[key_1].isCalculated = true;
		tree[key_2].isCalculated = true;
	}
}

map<string, string> huffman_codes(map<string, unsigned long long>& prob) {
	map<string, element> tree;
	map<string, string> result;
	for (map<string, unsigned long long>::iterator it = prob.begin(); it != prob.end(); ++it) {
		tree.insert(pair<string, element>(it->first, element()));
		tree[it->first].value = it->first;
		tree[it->first].amount = it->second;
	}

	huffman_tree(tree);

	string code;
	for (map<string, unsigned long long>::iterator it = prob.begin(); it != prob.end(); ++it) {
		code = "";
		element* iter = &(tree[it->first]);
		while (iter->number != "2") {
			code += iter->number;
			iter = iter->root;
		}
		unsigned long long n = code.length();
		for (int i = 0; i < n / 2; ++i)
			swap(code[i], code[n - i - 1]);
		result.insert(pair<string, string>(it->first, code));
	}
	return result;
}

pair<string, map<string, string>> huffman(const string& input, int symbolLength) {
	map<string, unsigned long long> prob;
	for (unsigned long long i = 0; i <= input.length() - symbolLength; i += symbolLength) {
		string temp(symbolLength, ' ');
		for (int j = 0; j < symbolLength; ++j) {
			temp[j] = input[i + j];
		}
		map<string, unsigned long long>::iterator elm = prob.find(temp);
		if (elm == prob.end()) {
			prob.insert(pair<string, int>(temp, 1));
		}
		else {
			++(elm->second);
		}
	}

	map<string, string> codes = huffman_codes(prob);

	string result;
	result.reserve(input.length());
	for (unsigned long long i = 0; i <= input.length() - symbolLength; i += symbolLength) {
		string temp(symbolLength, ' ');
		for (int j = 0; j < symbolLength; ++j) {
			temp[j] = input[i + j];
		}
		result += codes[temp];
	}
	return pair<string, map<string, string>> (result, codes);
}