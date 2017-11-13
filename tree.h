#ifndef tree_h
#define tree_h

#include <iostream>
#include <vector>

#define PRINT(str) std::cout << str
#define PRINTLN(str) PRINT(str) << std::endl

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
		int j = i;
		while (node && j) {
			node = node->nextSibling();
			j--;
		}
		
		return node;
	}
	
	void appendChild(TreeNode<T>* tn) {
		
		if (!m_firstChild)
			m_firstChild = tn;
		else {
		
			TreeNode<T>* mode = nullptr;
			TreeNode<T>* node = m_firstChild;
			
			while (node) {
				mode = node;
				node = node->nextSibling();
			}
			
			if (mode) {
				mode->setNextSibling(tn);
			}
		}		
	}
	
	void print(int depth = 0) {
		for (int i=0;i<depth;i++) {
			std::cout << "\t";
		}
		std::cout << data << std::endl;
		
		if (m_firstChild) {
			m_firstChild->print(depth + 1);
		}
		if (m_nextSibling) {
			m_nextSibling->print(depth);
		}
	}
			
};

template <typename Node> struct Tree {
	Node* m_root;
	
	Node* root() { return m_root; }
	void setRoot(Node* n) { m_root = n; }
	
	void print() {
		if (m_root) {
			m_root->print();
		}
	}
	
	// will destroy every node
	void destroy() {
		
	}
	
};

///////////////////////////

// xml tree-treenode

struct XMLTreeNode : public TreeNode<std::string> {

	XMLTreeNode() : TreeNode<std::string>() {}
	XMLTreeNode(const std::string& name) : TreeNode<std::string>(name) {}

};

struct XMLTree : public Tree<XMLTreeNode> {
	
};

#endif
