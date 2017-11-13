#ifndef xml_validator_h
#define xml_validator_h

class XMLValidator {
	
	

};

template <typename T> TreeNode {
	T data;
	TreeNode<T>* firstChild;
	TreeNode<T>* nextSibling;			
}

template <typename Data, Node> Tree {
	Node* root;
}

struct

struct XMLTreeNode : public TreeNode<std::string> {

};


struct XMLTree {

};

struct XMLTag {
	XMLTag(const bool& type, const std::string& name) : type(type), name(name) {}
	std::string name;
	bool type; 
}

#define START_ELEMENT 0
#define STOP_ELEMENT  1

// this actually evalutes well-formedness
bool parseXml(const std::string& path) {
	const std::vector<std::string>& lines;
	
	std::vector<std::pair<int, XMLTag>> elements;
	
	int currentDepth = 0;
	std::string currentElementName;
	bool currentElementType = true;
	
	for (auto& line : lines) {
		std::vector<std::string> content = split(line, ' ');
		if (content.size() == 2)  {
			currentElementType = std::stoi(content[0]);
			currentElementName = content[1];
			
			if (currentElementType == START_ELEMENT) {
				depth++;
				
			}
			else {
				depth--;
			}
			
			elements.push_back(std::pair<int, XMLTag>(depth, XMLTag(currentElementType, currentElementName)));
		}
		else {
			PRINTLN("Syntax error at line " + std::to_string(elements.size()));
			return false;
		}
	}
	
	// check if last element was a closing tag AND if the depth distribution is consistent
	// (as many closing elements as opening elements)
	if (currentElementType != STOP_ELEMENT || currentDepth) 
		return false;
		
	// checking for <a><b></a></b> incorrect tags
	
	std::vector<std::string> nestedElements;
	
	for (auto& pair : elements) {
		XMLTag& tag = pair.second;
		int& depth = pair.first;
		
		if (tag.type == START_ELEMENT) {
			nestedElements.push_back(tag.name);
		}
		else {
			if (nestedElements.size() && nestedElements[nestedElements.size()] == tag.name) {
				nestedElements.pop_back();
			}
			else {
				return false;
			}
		}
	}
	
	// there shouldn't be any element left in this list if depth is consistent
	if (nestedElements.size()) 
		return false;
	
	return true;
}
	

#endif // xml_validator_h
