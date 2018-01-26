# Jocelyn Vernay
# M2 Data & Knowledge
# Web Data Models - Project : DTD Validator for XML
# October 17th, 2017

import sys

##########################
#### USEFUL FUNCTIONS ####
##########################

# Output text content without a new line.
def printw(strs):
    sys.stdout.write(strs)
    sys.stdout.flush()
    
# Parse the file and return a list containing the lines of that file.
def getLines(filepath):
    f = open(filepath)
    a = f.readlines()
    f.close()
    return a

# Extract the text between a couple of parenthesis
# e.g. 'a(bc)*d', to get the sub-element 'bc'
# assuming 'i' is the index of the first parenthesis
def extractSubRegex(regex, i):
    nested = 0
    for j in range(i + 1, len(regex)):
        if regex[j] == '(':
            nested = nested + 1
        if regex[j] == ')':
            if (nested < 1):
                return regex[i+1:j]
            nested = nested - 1
    return ""


##########################
####### CONSTANTS ########
##########################

# Simply the character used by default to identify the empty word.
EPSILON = ' '

# When exploring an automaton, the algorithm is allowed to pass 
# through epsilon relations a maximum of MAX_EPSILON times 
# successively.
# This is done so to prevent infinite loops in the automaton. This 
# is reset every time we pass a relation holding non-empty data.
# Used in the recursive 'pathExists' method
MAX_EPSILON = 2

# Those two are used in the XML parser to make the code clearer.
START_ELEMENT = 0
STOP_ELEMENT = 1

##########################
###### PARSING XML #######
##########################

# This function checks for well-formedness in the XML document. 
# Well-formedness in our case follows three criteria : 
# 1. One and only one root element
# 2. A closing element should follow an opening element of the same 
#    name, directly or nested (see 'nestedElements')
# 3. Have as much stop elements as there are start elements (evaluated 
#    by 'depth')
# Tree building is handled by another function (xmlTreeFromLines).
def wellFormed(lines):
    
    depth = 0
    previousType = True
    
    # Indicates whether we have already encountered a root element
    oneRoot = False
    
    nestedElements = []
    
    for line in lines:
        content = line.split(' ')
        
        elementType = int(content[0])
        elementName = content[1].strip()
        
        if elementType == START_ELEMENT:
            if depth == 0:
                if not oneRoot:
                    oneRoot = True
                else:
                    #print("There is more than one root element.")
                    return False
            depth = depth + 1
            nestedElements.append(elementName)
        else:
            if depth == 0:
                #print("Can't begin by a closing element !")
                return False
            if len(nestedElements) > 0 and nestedElements[len(nestedElements) - 1] == elementName:
                nestedElements.pop()
            else:
                return False
            depth = depth - 1
        
        if (depth < 0):
            return False
        
        previousType = elementType
        
    return depth == 0 and len(nestedElements) == 0 and previousType == STOP_ELEMENT

# A simple tree node structure.
class TreeNode:
    def __init__(self, name):
        self.children = []
        self.name = name
        
    def appendChild(self, node):
        self.children.append(node)
        #print(node.name + " has been adopted by " + self.name + " !")
    
    def child(self, i):
        if i < len(self.children) and i >= 0:
            return self.children[i]
        else:
            return None

# A simple tree structure.
class XMLTree:
    def __init__(self):
        self.root = None
        
    def setRoot(self, node):
        self.root = node

# This function builds a tree for a XML document. It is assumed that 
# well-formedness of the content has been checked using the eponymous 
# function before using this function, as it assumes it is well-formed
def xmlTreeFromLines(lines):
    
    currentNode = None
    parentNode = None
    nestedElements = []
    rootNode = None
    
    for l in lines:
        content = l.split(' ')
        elementType = int(content[0])
        elementName = content[1].strip()
        
        if elementType == START_ELEMENT:
            if currentNode == None: # root node
                currentNode = rootNode = TreeNode(elementName)
            else:
                if parentNode != None:
                    nestedElements.append(parentNode)
                    
                parentNode = currentNode
                currentNode = TreeNode(elementName)
                parentNode.appendChild(currentNode)
        else: # stop element # backtrack
            currentNode = parentNode
            if len(nestedElements) > 0:
                parentNode = nestedElements[len(nestedElements) - 1]
                nestedElements.pop()
            else:
                parentNode = None
            
    tree = XMLTree()
    tree.root = rootNode
    
    return tree

###########################
### REGULAR EXPRESSIONS ###
###########################

# An automaton Node. It is connected to other nodes by relations 
# holding data, which for this project is one char, namely the 
# name of the element appearing in the regular expression.
class RegexAutomatonNode:
    def __init__(self):
        self.relations = []
    
    # Add a relation from this node to 'dest' node, with the data 'data'
    def addRelation(self, data, dest):
        self.relations.append((data,dest))
        
    # Retrieve all the relations holding the data 'data'. 
    # The automaton is not deterministic.
    def getRelationsFromData(self, data):
        return [tu for tu in self.relations if tu[0] == data]
    
    # Output this node's relations.
    def displayRelations(self):
        for r in self.relations:
            printw(r[0] + " ")
        printw("\n")
    
    # Search a path for the string 'value' in the automaton.
    # As explained above (see MAX_EPSILON), to avoid loops through 
    # epsilon relations, the program allows to pass through an 
    # epsilon relation a maximum of MAX_EPSILON times in a row.
    def pathExists(self, value, epsilons):
        
        if (len(value) == 0):
            return True
        
        # If the relevant character is found, recursion is called on 'value' minor that character.
        relations = self.getRelationsFromData(value[0])
        for r in relations:
            if r[1].pathExists(value[1:], MAX_EPSILON):
                return True
            
        # Epsilon relations have no cost.
        if epsilons > 0:
            relations = self.getRelationsFromData(EPSILON)
            for r in relations:
                if r[1].pathExists(value, epsilons - 1):
                    return True
        
        return False

# A structure encapsulating the nodes.
class RegexAutomaton:
    def __init__(self):
        self.root = None
    
    def setRoot(self, node):
        self.root = node
        
    # Load the regular expression.
    def load(self, regex):
        
        auto = generateElementAutomaton(regex)
        
        if (auto == None or (auto.first != None and auto.last == None)):
            return False
        else:
            self.root = auto.first
            return True
        
    # Tells whether the string 'value' is generated by this automaton.
    def validate(self, value):
        if self.root != None:
            return self.root.pathExists(value, MAX_EPSILON)
        else:
            return True

# A structure used to build the regular expression automaton. 
# Each element and sub-element is identified as a sub-automaton 
# (first and last nodes), having a special behavior (*, ?, +). 
# 'text' is used for outputs.
class Element:
    def __init__(self):
        self.first = None
        self.last = None
        self.special = None
        self.text = None

# Recursive function used to parse a regular expression and build the corresponding automaton.
def generateElementAutomaton(regex):
    
    auto = Element()
    
    elements = []
    
    if len(regex) == 0 or regex == '_':
        return auto
    
    i = 0
    while i < len(regex):
        c = regex[i]
       
        if c == '_':
            return auto
        elif c == '(':
            subregex = extractSubRegex(regex, i)
            subauto = generateElementAutomaton(subregex)
            subauto.text = subregex
            elements.append(subauto)
            i = i + len(subregex) + 1
        elif c == ')':
            return None
            continue
        elif c == '+' or c == '?' or c == '*':
            if (len(elements) > 0):
                elements[len(elements) - 1].special = c 
            else:
                #print("Syntax error")
                return None
        else: # a-z
            subauto = Element()
            subauto.first = RegexAutomatonNode()
            subauto.last = RegexAutomatonNode()
            subauto.first.addRelation(c, subauto.last)
            subauto.text = c
            elements.append(subauto)
            
        i = i + 1
    #endfor
    
    if (len(elements) == 1 and elements[0].special == None):
        return elements[0]
    
    currentRegexAutomatonNode = None
    nextRegexAutomatonNode = None
    
    for i in range(len(elements)):
        
        if auto.first == None: # first element
            
            auto.first = elements[i].first
            currentRegexAutomatonNode = auto.first
            nextRegexAutomatonNode = elements[i].last
            
        else: # middle element
            currentRegexAutomatonNode.addRelation(EPSILON, elements[i].first)
            nextRegexAutomatonNode = elements[i].last
            
        # special behaviors
        if nextRegexAutomatonNode != None and currentRegexAutomatonNode != None:
            if (elements[i].special == '*' or elements[i].special == '+'):
                nextRegexAutomatonNode.addRelation(EPSILON, currentRegexAutomatonNode)
                
            if (elements[i].special == '*' or elements[i].special == '?'):
                currentRegexAutomatonNode.addRelation(EPSILON, nextRegexAutomatonNode)
                
        currentRegexAutomatonNode = nextRegexAutomatonNode
                
    auto.last = currentRegexAutomatonNode
    
    return auto

## Regex parser tests
# This is used to evaluate the accuracy of the regex loading method.
def loadTests(filepath):
    lines = getLines(filepath)
    
    print("\n" + filepath)
    print("%15s %15s %15s %15s %15s" % ("REGEX", "VALUE", "EXPECTED", "OBSERVED", "RESULT"))
    [printw('-') for i in range(15*5 + 4)]
    print("")
    for line in lines:
        if (len(line) == 0 or (len(line) > 0 and line[0] == '#')):
            continue
        if len(line) > 0 and line[0] == ';':
            break

        content = line.split(' ')
        
        if len(content) < 2:
            continue
        
        regex = content[0]
        value = content[1]
        expected = int(content[2])
        
        auto = RegexAutomaton()
        
        if (not auto.load(regex)):
            print("Invalid regex : " + regex)
            continue
        
        observed = auto.validate(value)

        ans = "OK" if observed == expected else "KO<"

        print("%15s %15s %15s %15s %15s" % (regex, value, expected, int(observed), ans))
        

##########################
### VALIDATING WITH DTD ##
##########################

class DTDValidator:
    def __init__(self):
        self.expressions = {}
        
    def addExpression(self, element, automaton):
        self.expressions[element] = automaton
        
    def parseExpression(self, line):
        content = line.split(' ')
        
        if len(content) != 2:
            return False
        
        element = content[0]
        regex = content[1]
        
        auto = RegexAutomaton()
        
        
        if not auto.load(regex):
            return False
        
        self.addExpression(element, auto)
        return True
    
    
    def validate(self, node):

        # There is a rule for this element
        if node.name in self.expressions:

            regexAutomaton = self.expressions[node.name]

            #The element is supposed to be empty, but it is not.
            if (regexAutomaton.root == None and len(node.children) > 0):
                return False
            
            # The element is supposed to be empty, and is empty.
            elif (regexAutomaton.root == None and len(node.children) == 0):
                return True
            
            # The element is not empty, nor should it be.
            else:
                content = ""
                for childNode in node.children:
                    content = content + childNode.name

                # this node is valid, but we need to check on its children
                if regexAutomaton.validate(content):
                    #print(": valid")
                    for childNode in node.children:
                        # If a children is not valid, then the entire tree is made invalid
                        if not self.validate(childNode):
                            #print(childNode.name + " brought shame to the family !")
                            return False
                else:
                    #print(": not valid")
                    return False
            return True

        # There is no rule for this element, so it is valid
        else:
            return True

# generates a dtd validator from text content
def dtdValidatorFromLines(lines):
    terminator = DTDValidator()
    for l in lines:
        terminator.parseExpression(l)
    return terminator


## ^ Data structures and algorithms
#########################################
#########################################
## v Actual code
    
# assessing the regex parser
#loadTests("tests/test_brackets")
#loadTests("tests/test_loop")
#loadTests("tests/test_skip")
#loadTests("tests/test_loopXskip.txt")


#  Result :
# 0 -> not well-formed & not valid
# 1 -> well-formed & not valid
# 2 -> well-formed & valid
def validateXMLwidhDTDfromLines(xml, dtd):
    # is it well formed ?
    if wellFormed(xml):
        # if it is, build a tree
        xmlTree = xmlTreeFromLines(xml)
        #check dtd validity :
        dtdValidator = dtdValidatorFromLines(dtd)

        if xmlTree.root != None and dtdValidator.validate(xmlTree.root):
            return 2
        else:
            return 1
    else:
        return 0

def validateXMLwithDTD(xmlPath, dtdPath):
    xml = getLines(xmlPath)
    dtd = getLines(dtdPath)
    return validateXMLwidhDTDfromLines(xml, dtd)
    


# main
if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print("not well-formed")
        print("not valid")
        exit()

    ans = validateXMLwithDTD(sys.argv[1], sys.argv[2])

    if ans > 0:
        print("well-formed")
    else:
        print("not well-formed")

    if ans > 1:
        print("valid")
    else:
        print("not valid")


    

    