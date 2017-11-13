#ifndef regex_h
#define regex_h

#include "automata.h"
#include "tree.h"

// Regex Automata

struct RegexAutomataNode : public AutomataNode<std::string, RegexAutomataNode> {
	
};

struct RegexAutomata : public Automata<RegexAutomataNode> {
	
};

// Regex Tree for parsing

struct RegexTreeNode : public TreeNode<std::string> {
	RegexTreeNode() : TreeNode<std::string>() {}
	RegexTreeNode(const std::string& name) : TreeNode<std::string>(name) {}
};

struct RegexTree : public Tree<RegexTreeNode> {
	
};

//

// this cannot handle nested buffers.
struct ParseBuffer {
	std::vector<std::string> elements;
	std::string currentElement = "";
	
	void flush() {
		elements.push_back(currentElement);
		currentElement = "";
	}
	
	void add(const std::string& a) {
		currentElement += a;
	}
	
	void add(const char& ch) {
		currentElement += ch;
	}
	
	std::vector<std::string> getElements() {
		return elements;
	}
	
	std::string get() {
		return currentElement;
	}
	
	void push(const std::string& str) {
		flush();
		elements.push_back(str);
	}

	void push(const char& ch) {
		flush();
		elements.push_back("" + ch);
	}
	
	int count() {
		return elements.size();
	}
};

template <typename Node> struct TreeParser {
	Node* rootNode = nullptr;
	Node* currentParentParent = nullptr;
	Node* currentParent = nullptr;
	Node* currentNode = nullptr;
	
	TreeParser() {
		
	}
	
	TreeParser(Node* n) {
		setRoot(n);
	}
	
	std::vector<Node*> stack;
	
	void setRoot(Node* n) {
		rootNode = n;
		currentNode = n;
		currentParent = n;
	}
	
	Node* root() {
		return rootNode;
	}
	
	bool createLeaf(Node* n) {
		if (n && currentParent) {
			currentParent->appendChild(n);
			currentNode = n;
			return true;
		}
		else {
			return false;
		}
	}
	
	bool createBranch(const std::vector<Node*>& nodes) {
		if (nodes.size()) {
			Node* node = currentParent;
			
			for (int i=0; i <nodes.size(); i++) {
				if (nodes[i] && node) {
					node->appendChild(nodes[i]);
					node = nodes[i];
				}
			}	
			
		}
	}
	
	void forward(Node* n) {
		
		if (n != currentNode) {
			if (!createLeaf(n)) 
				return;
		}
		
		if (currentParentParent) {
			stack.push_back(currentParentParent);
		}
		
		currentParentParent = currentParent;
		
		currentParent = n;
		
		currentNode = n;
	}
	
	// not updated
	void backward(const int& offset = 1) {
		
		for (int i=0; i<offset && currentParent && stack.size() > 0; i++) {
			currentNode = currentParent;
			currentParent = stack[stack.size() - 1];
			stack.pop_back();
		}
		
	}
	
};

//

std::string whitespace(const std::string& str) {
	return str;
};

struct Regex : public Automata<RegexAutomataNode> {
	
	Regex(const std::string& str);
	bool parse(const std::string& str);
	
};

#endif // regex_h