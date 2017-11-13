#include <iostream>
#include <sstream>

//#include "xml_validator.h"

//#include "tree.h"

#include "regex.h"

bool wellFormed(const std::string& xml);
// xml header
// correct syntax
// consistent tags (not <a><b></a></b>)

bool dtdValid(const std::string&  xml, const std::string& dtd);

int main(int argc, char** argv) {
    if (argc < 3) {
//        PRINTLN("Invalid arguments.");
        return 0;
    }

    std::string xml = argv[1];
    std::string dtd = argv[2];

//	XMLTree t;
//	XMLTreeNode n;
	
//	n.setFirstChild(&n);

	std::string str ="a(bc)*d";

	Regex r(str);
	
	std::cout << r;

	return 0;

}
