#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck() {
    deleteNode(m_head);
    
}
void StudentSpellCheck::deleteNode(Node* z)
{
    for(std::vector<Node*>:: iterator it=z->m_children.begin(); it!=z->m_children.end();)
    {
        deleteNode(*it);
        it=z->m_children.erase(it);
    }
    delete z;
}
bool StudentSpellCheck::load(std::string dictionaryFile) {
    
    std::string x;
//    std::ifstream infile("/Users/rafimoldovsky/Desktop/Wurd/Wurd/sample.txt");
    std::ifstream infile(dictionaryFile);
    if(!infile)
    {
        return false;
    }
    char c = '\'';
    while(getline(infile, x))
    {
        for(int i=0; i<x.size();i++) //this has an upper bound so there is no contribution to the order
        {
            if(!isalpha(x[i]) && x[i]!=c)
            {
                x.erase(i,1);
                i--;
            }
        }
        if(x.size()>0)
            insertOntoTrie(x);
    }
//    printOutTrie(m_head);
//    if(findStringOnTrie("first"))
//    std::cerr<<"noice";
    return true;
    //ISN'T THIS ORDER NLOGN? BECAUSE INSERTING IS LOGN? -where N is the number of keys though- still related to number of strings-ASK IN OH
}
bool StudentSpellCheck::isIn(std::string word, std::vector<std::string>& words)const
{
    for(std::vector<std::string>::const_iterator it=words.begin(); it!=words.end(); it++)
    {
        if(*it==word)
            return true;
    }
    return false;
}
bool StudentSpellCheck::FindSuggestions(Node* z,std::string word, std::string &suggestion, bool error,std::vector<std::string>& suggestions, int suggestionsLeft) const
{
    if(word.size()==0  && !isIn(suggestion,suggestions))
    {
        for(std::vector<Node*>::const_iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
        {
            Node* temp= *it;
            if(temp->m_value=='.')
            {
                if(suggestionsLeft==1)
                {
                    suggestions.push_back(suggestion);
                    return true;
                }
                else
                {
                    suggestionsLeft--;
                    suggestions.push_back(suggestion);
                    return false;
                }
            }
        }
        return false;
    }
    else if(word.size()==0)
        return false;
    std::string startSuggestion=suggestion;
    std::string startWord=word;
    for(std::vector<Node*>::const_iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
    {
        Node* temp=*it;
        if(temp->m_value==tolower(word[0]))
        {
            suggestion+=tolower(word[0]);
            
            word.erase(0,1);
            if(FindSuggestions(*it, word, suggestion,error,suggestions,suggestionsLeft))
               return true;
            suggestion=startSuggestion;
            word=startWord;
        }
    }
    if(!error)
    {
        error=true;
        startSuggestion=suggestion;
        startWord=word;
        for(std::vector<Node*>::const_iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
        {
            Node* temp=*it;
            if(temp->m_value!='.')
            {
                suggestion+= temp->m_value;
                word.erase(0,1);
                if(FindSuggestions(*it, word, suggestion,error,suggestions,suggestionsLeft))
                    return true;
                suggestion=startSuggestion;
                word=startWord;
            }
        }
    }
    return false;
}
bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
        if(findStringOnTrie(word))
            return true;
    while(!suggestions.empty())
        suggestions.pop_back();
//    for(int i=0; i<max_suggestions; i++)
//    {
//        std::string x="";
//        if(FindSuggestions(m_head, word, x, false, suggestions))
//            suggestions.push_back(x);
//        else
//            return false;
//    }
    std::string x="";
    FindSuggestions(m_head, word, x, false, suggestions, max_suggestions);
    return false;
    //THE ORDER HERE IS CONFUSING- ASK IN OH
//    if(findStringOnTrie(word))
//        return true;
//    Node* z=m_head;
//    for(int j=0; j<word.size();j++)
//    {
//       for(std::vector<Node*>::iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
//        {
//            Node* temp = *it;
//            std::string tempWord= word;
//            tempWord[j]=temp->m_value;
//            if(findStringOnTrie(tempWord))
//            {
//                suggestions.push_back(tempWord);
//                if(suggestions.size()==max_suggestions)
//                    return false;
//            }
//        }
//        bool found=false;
//        //now we have to move on to the next letter being anything
//        for(std::vector<Node*>::iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
//        {
//            Node* temp = *it;
//            if(temp->m_value==word[j])
//            {
//                z=temp;
//                found=true;
//            }
//        }
//        if(!found)//this means that this character has to be the one that is different
//            return false;
//    }
//    return false;
//    //THE ORDER OF THIS BE ALL MESSED UP
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    char c = '\'';
    int startWord=0;
    int i;
    bool foundLetter=false;
    while(!problems.empty())
        problems.pop_back();
    for(i=0; i<line.size();i++)
    {
        if(!isalpha(line[i]) && line[i]!=c)
           {
               std::string temp=line.substr(startWord,i-startWord);
               if(foundLetter && !findStringOnTrie(temp))
               {
                   Position* positionTemp = new Position();
                   positionTemp->start=startWord;
                   positionTemp->end=i-1;//is this correct  if its inclusive?
                   problems.push_back(*positionTemp);
                   
               }
               while(i<line.size() && !isalpha(line[i]) && line[i]!=c)
               {
                   i++;
               }
               startWord=i;
               i--;
               foundLetter=false;
           }
        else if(!foundLetter)
            foundLetter=true;
    }
    std::string temp=line.substr(startWord,i-startWord);
    if(foundLetter && startWord!=line.size()-1 && !findStringOnTrie(temp))
    {
        Position* positionTemp = new Position();
        positionTemp->start=startWord;
        positionTemp->end=i-1;//is this correct  if its inclusive?
        problems.push_back(*positionTemp);

    }
    //THE ORDER HERE IS WEIRD- ISN'T FINDSTRINGONTRIE ORDER LOGN? WHERE N IS THE NUMBER OF LETTERS OR IS IT JUST N? ASK IN OH
}
bool StudentSpellCheck::insertOntoTrie(Node* z,std::string s)
{
    if(s.size()==0) //going to check that it's not already on it :)
    {
        for(std::vector<Node*>::iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
        {
                Node* temp= *it;
            if(temp->m_value=='.')
                return false;
        }
        Node* last = new Node;
        last->m_value='.';
        z->m_children.push_back(last);
        return true;
    }
    for(std::vector<Node*>::iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
    {
        Node* temp= *it;
        if(temp->m_value==tolower(s[0]))
        {
            s.erase(0,1);
            return insertOntoTrie(*it, s);
        }
    }
    Node* add = new Node;
    add->m_value=tolower(s[0]);
    z->m_children.push_back(add);
    s.erase(0,1);
    return insertOntoTrie(add, s);
}
bool StudentSpellCheck::insertOntoTrie(std::string s)
{
    return insertOntoTrie(m_head,s);

//    Node* p;
//    if(m_head==nullptr)
//    {
//        m_head = new Node;
//        p=m_head;
//        for(int i=0; i<s.size();i++)
//        {
//            Node* x = new Node;
//            x->m_value=tolower(s[i]);
//            p->m_children.push_back(x);
//            p=x;
//        }
//        Node* last = new Node;
//        last->m_value='.';
//        p->m_children.push_back(last);
//        return true;
//    }
//    Node* z= m_head;
//    int element=0;
//    for(std::vector<Node*>::iterator it=z->m_children.begin(); it!=z->m_children.end() && element<s.size()-1;it++)
//    {
//        Node* temp= *it;
//        if(temp->m_value==s[element])
//        {
//            z=temp;
//            it=z->m_children.begin();
//            element++;
//        }
//        if(element==s.size()-1)
//        {
//            Node* last = new Node;
//            last->m_value='.';
//            z->m_children.push_back(last);
//            return true;
//        }
//    }
//    for(int i=element; i<s.size(); i++)
//    {
//        Node* add= new Node;
//        add->m_value=s[i];
//        z->m_children.push_back(add);
//        z=add;
//    }
//    Node* last = new Node;
//    last->m_value='.';
//    z->m_children.push_back(last);
//    return true;
}
bool StudentSpellCheck::findStringOnTrie(Node* z, std::string s) const
{
    if(s.size()==0 && z->m_value=='.')
        return true;
    else if(s.size()==0)
        return false;
    if(z->m_value==tolower(s[0]))
    {
        for(std::vector<Node*>::const_iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
        {
            if(s.size()==1)
            {
                if(findStringOnTrie(*it, ""))
                    return true;
            }
            else if(findStringOnTrie(*it, s.substr(1)))
                    return true;
        }
    }
    
        return false;
}
bool StudentSpellCheck::findStringOnTrie(std::string s) const
{

    for(std::vector<Node*>::const_iterator it=m_head->m_children.begin(); it!=m_head->m_children.end();it++)
        if(findStringOnTrie(*it, s))
            return true;
    return false;
//    std::cerr<<"huh";
//    Node* z= m_head;
//    int element=0;
//    for(std::vector<Node*>::iterator it=z->m_children.begin(); it!=z->m_children.end() && element<s.size()-1 && !z->m_children.empty();it++)
//    {
//        std::cerr<<element;
//        Node* temp= *it;
//        std::cerr<<temp->m_value<< s[element]<< " ";
//        if(temp->m_value==s[element])
//        {
//            std::cerr<<"*";
//            z=temp;
//            it=z->m_children.begin();
//            element++;
//        }
//    }
//    if(element==s.size()-1)
//    {
//        std::cerr<<"here";
//        for(std::vector<Node*>::iterator it=z->m_children.begin(); it!=z->m_children.end() && element<s.size()-1 && !z->m_children.empty();it++)
//        {
//            Node* temp= *it;
//            if(temp->m_value=='.')
//            {
//                std::cerr<<"yeet";
//                return true; //have to have this for loop in order to make sure this actual word is contained
//            }
//        }
//    }
//
//    std::cerr<<"hmm";
//    return false;
    
}
void StudentSpellCheck::printOutTrie(Node* z) const
{
    if(z->m_value=='@')
        std::cerr<<"what!";
    if(z->m_children.empty())
    {
        std::cerr<<z->m_value<<std::endl;
    }
    for(std::vector<Node*>::const_iterator it=z->m_children.begin(); it!=z->m_children.end();it++)
    {
        std::cerr<<z->m_value;
        printOutTrie(*it);
    }
}
