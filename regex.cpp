#include "regex.h"


#define IS_NUMBER(ch) (ch >= '0' && ch <= '9')
#define IS_UPPER(ch) (ch >= 'A' && ch <= 'Z')
#define IS_LOWER(ch) (ch >= 'a' && ch <= 'z')
#define IS_LETTER(ch) (IS_LOWER(ch) || IS_UPPER(ch))
#define IS_ALPHANUMERIC(ch) (IS_LETTER(ch) || IS_NUMBER(ch))
#define ERROR println("ERROR"); return;

//template <typename T> bool contains(const std::vector<T>& vec, T element) {
//	return std::find(vec.begin(), vec.end(), element) != vec.end();
//}

bool contains(const std::string& str, const char& ch) {
	for (auto& cat : str) {
		if (cat == ch) {
			return true;
		}
	}
	return false;
}

Regex::Regex(const std::string& str) {
	parse(str);
}

bool Regex::parse(const std::string& str) {
	
//		str = whitespace(str);
	
//	std::vector<char> allowedChars = {'*', '_', '?', '+', '(', ')'};
	std::string allowedChars = "*_?+()";
	std::string specialChars = "*?+";
//	std::vector<char> specialChars = {'*', '?', '+'};
	
	for (auto& ch : str) {
		if (!IS_ALPHANUMERIC(ch) && !contains(allowedChars, ch)) {
			PRINTLN(str + ": " + "Incorrect regex.");
			return;
		}
	}
	
	if (!str.length() || str == "_") {
		return;
	}
	else {
		// decrypt regex
		
		// look sequentially
		
		// we admit here that every element is atomic (one character)
		// ab >> 'a'  then 'b'
		
		RegexTreeNode root;
		
		TreeParser<RegexTreeNode> treeParser(&root);
		
		// rather than parentheses, we would have to test whether the current parent
		// node is of type '()'
		bool openParatheses = false;
		
		for (int i=0; i<str.length(); i++) {
			if (IS_ALPHANUMERIC(str[i])) {
				
					// pbuffer.push(str[i]);
					// langoose :
					// check if next element is special char
					// TRUE -> add as a leaf of a node containing the special char (it being a child of the current parent node)
					// AND i++ ?
					// FALSE -> add as a leaf to current parent node
					
					//if (i < str.length() - 1 && contains<char>(specialChars, str[i+1])) {
					//	treeParser.createBranch({
					//		new RegexTreeNode("" + str[i+1]), 
					//		new RegexTreeNode("" + str[i])
					//	});
					//}
					//else {
					//	treeParser.createLeaf(new RegexTreeNode("" + str[i]));
					//}
					
					// append new node to current parent
					/// This becomes the currentNode
					treeParser.createLeaf(new RegexTreeNode("" + str[i]));

			}
//				else if (contains<char>(allowedChars, str[i])) {
			else if (contains(allowedChars, str[i])) {
				switch(str[i]) {
					case '(':
						// create and descend new node
						treeParser.forward(new RegexTreeNode("()"));
					break;
					case ')':
						// backtrack up + see:langoose
						// more complicated : you'll have to rebase the current node
						treeParser.backward();
					break;
					case '*':
					case '+':
					case '?':
						// spec
						
						
						if (!i) {
							return false;
						}
						
						// rebase
						
					break;
					default:
					case '_':
						// error
						//ERROR;
					break;
				}
			}
			else {
				PRINTLN("Unknown char.");
			}
		}
		
//		std::vector<std::string> elements = pbuffer.getElements();
//		
//		// build the tree
//		for (auto& element : elements) {
//			
//			if (element is a special char) {
//				if (there was a previous element) {
//					
//				}
//				else {
//					ERROR;
//				}
//			}
//			
//			else if (element is an atomic element) {
//				
//			}
//			
//			else if (element is parantheses-encased) {
//				// remove parantheses
//				
//				for (int i=0; i<element.length(); i++) {
//					// for each character create leaf
//				}
//				
//			}
//			
//		}
		
		// build automata
		
		return true;
		
	}
}
