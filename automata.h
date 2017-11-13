#ifndef automata_h
#define automata_h

#include <vector>

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

#endif // automata_h