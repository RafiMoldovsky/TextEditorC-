#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() { }
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
struct Node
    {
        char m_value;
        std::vector<Node*> m_children;
    };
    bool insertOntoTrie(std::string s);
    bool insertOntoTrie(Node* z, std::string s);
    Node* m_head = new Node; //is this ok?
    bool findStringOnTrie(std::string s) const;
    bool findStringOnTrie(Node* z, std::string s) const;
    void printOutTrie(Node* z) const;
    bool FindSuggestions(Node* z,std::string word, std::string &suggestion, bool error,std::vector<std::string>& suggestions, int suggestionsLeft) const;
    bool isIn(std::string word, std::vector<std::string>& words)const;
    void deleteNode(Node* z);
};

#endif  // STUDENTSPELLCHECK_H_
