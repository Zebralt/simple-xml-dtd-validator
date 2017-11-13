#ifndef tree_h
#define tree_h

#include <iostream>
#include <vector>

// generic  tree-treenode class

template <typename T> struct TreeNode {
	T data;
	TreeNode<T>* m_firstChild;
	TreeNode<T>* m_nextSibling;
	
	TreeNode<T>* firstChild() {
		return m_firstChild;
	}
	
	TreeNode<T>* nextSibling() {
		return m_nextSibling;
	}
	
	void setFirstChild(TreeNode<T>* fc) {
		m_firstChild = fc;
	}
	
	void setNextSibling(TreeNode<T>* ns) {
		m_nextSibling = ns;
	}
	
	void setData(const T& t) {
		data = t;
	}
	
	TreeNode() {
		
	}
	
	TreeNode(const T& t) {
		data = t;
	}
	
	int childCount() {
		int t = 0;
		TreeNode<T>* node = m_firstChild;
		
		while (node) {
			node = node->nextSibling();
			t++;
		}
		
		return t;
	}
	
	TreeNode<T>* child(const int& i) {
		TreeNode<T>* node = m_firstChild;
		while (node && i) {
			node = node->nextSibling();
			i--;
		}
		
		return node;
	}
	
	void appendChild(TreeNode<T>* 
			
};

template <typename Node> struct Tree {
	Node* m_root;
	
	Node* root() { return m_root; }
	void setRoot(Node* n) { m_root = n; }
};

// xml tree-treenode

struct XMLTreeNode : public TreeNode<std::string> {

	XMLTreeNode() : TreeNode<std::string>() {}
	XMLTreeNode(const std::string& name) : TreeNode<std::string>(name) {}

};

struct XMLTree : public Tree<XMLTreeNode> {
	
};

// generic Automata-AutomataNode classes

template <typename Relation, typename RelationData> struct AutomataNode {
	std::vector<std::pair<RelationData, Relation>> relations;
	
	Relation* getRelationFromData(RelationData rd) {
		for (auto& pair : relations) {
			if (rd == pair.fisrt) {
				return pair.second;
			}
		}
	}
};


template <typename AutomataNode> struct Automata {
	AutomataNode start;	
};

// Regex Automata

struct RegexAutomataNode : public AutomataNode<std::string, RegexAutomataNode> {
	
};

struct RegexAutomata : public Automata<RegexAutomataNode> {
	
}

// Regex Tree for parsing

struct RegexTreeNode : public TreeNode<std::string> {
	RegexTreeNode() : TreeNode<std::string>() {}
	RegexTreeNode(const std::string& name) : TreeNode<std::string>(name) {}
}

struct RegexTree : public Tree<RegexTreeNode> {
	
}

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
}

template <typename Node> struct TreeExplorer {
	std::vector<Node*> stack;
	
	
	
	Node* parent() {
		return parentNode;
	}
	
	Node* current() {
		return currentNode;
	}
	
	
}

//

std::string whitespace(const std::string& str) {
	return str;
}

#define IS_NUMBER(ch) (ch >= '0' && ch <= '9')
#define IS_UPPER (ch) (ch >= 'A' && ch <= 'Z')
#define IS_LOWER (ch) (ch >= 'a' && ch <= 'z')
#define IS_LETTER(ch) (IS_LOWER(ch) || IS_UPPER(ch))
#define IS_ALPHANUMERIC(ch) (IS_LETTER(ch) || IS_NUMBER(ch))
#define ERROR 

template <typename T> bool contains(const std::vector<T>& vec, T element) {
	std::vector<T>::const_iterator it = std::find(vec.begin(), vec.end(), element);
	return it != vec.end();
}

struct Regex : public Automata<RegexNode> {
	
	Regex(const std::string& str) {
		
		str = whitespace(str);
		
		std::vector<char> allowedChars = {'*', '_', '?', '+', '(', ')'};
		
		for (auto& ch : str) {
			if (!IS_ALPHANUMERIC(ch) && !contains<char>(allowedChars, ch)) {
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
			
			ParseBuffer pbuffer;
			
			RegexTreeNode root;
			RegexTreeNode* currentParentNode = &root;
			RegexTreeNode* currentNode = &root;
			
			// rather than parentheses, we would have to test whether the current parent
			// node is of type '()'
			bool openParatheses = false;
			
			for (int i=0; i<str.length(); i++) {
				if (IS_ALPHANUMERIC(str[i])) {
					if (openParantheses) {
						// pbuffer.add(str[i]);
						
						// add as a leaf to current parent node
						// keep same parent node
						// + langoose
						// essentially the same as below
					}
					else {
						// pbuffer.push(str[i]);
						// langoose :
						// check if next element is special char
						// TRUE -> add as a leaf of a node containing the special char (it being a child of the current parent node)
						// AND i++ ?
						// FALSE -> add as a leaf to current parent node
					}
				}
				else if (contains<char>(allowedChars, str[i])) {
					switch(str[i]) {
						case '(':
							// create and descend new node
						
							// open element
							if (openParantheses) {
								ERROR;
							}
							else {
								openParantheses = true;
								pbuffer.flush();
								pbuffer.add("(");
							}
						break;
						case ')':
							// backtrack up + see:langoose
						
							// if no open element, error
							// close element
							if (!openParantheses) {
								ERROR;
							}
							else {
								openParantheses = false;
								pbuffer.add(")");
								pbuffer.flush();
							}
						break;
						case '*':
							// check if current element != null
							// else error
							// if true then label
							if (!pbuffer.count()) { // no previous element
								ERROR;
							}
							else {
								pbuffer.push("*");
							}
						break;
						case '+':
							// check if current element != null
							// else error
							// if true then label
							if (!pbuffer.count()) { // no previous element
								ERROR;
							}
							else {
								pbuffer.push("+");
							}
						break;
						case '?':
							// check if current element != null
							// else error
							// if true then label
							if (!pbuffer.count()) { // no previous element
								ERROR;
							}
							else {
								pbuffer.push("?");
							}
						break;
						default:
						case '_':
							// error
							ERROR;
						break;
					}
				}
				else {
					PRINTLN("Unknown char.");
				}
			}
			
			std::vector<std::string> elements = pbuffer.getElements();
			
			// build the tree
			for (auto& element : elements) {
				
				if (element is a special char) {
					if (there was a previous element) {
						
					}
					else {
						ERROR;
					}
				}
				
				else if (element is an atomic element) {
					
				}
				
				else if (element is parantheses-encased) {
					// remove parantheses
					
					for (int i=0; i<element.length(); i++) {
						// for each character create leaf
					}
					
				}
				
			}
			
			// build automata
			
		}
	}
	
};


///////////////////////////

#endif
