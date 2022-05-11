#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
TextEditor* createTextEditor(Undo* un)
{
    return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
     m_currentCol=0;
     m_currentRow=0;
     m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
     m_size=0;
     insertAtBack(""); //The starting string- the size is now one
     m_currentRowPtr=m_head->m_next;
     //THIS IS ORDER 1
 }

StudentTextEditor::~StudentTextEditor()
{
    // Delete all Nodes from first non-dummy up to but not including
          // the dummy

        while (m_head->m_next != m_head)
            doErase(m_head->m_next);

          // Delete the dummy
        //shouldn't need to delete the m_currentRowPtr right?
        delete m_head;
//    if(getUndo()!=nullptr)
//        delete getUndo(); //could also just use reset probably
    //THE WHILE LOOP IS THE ORDER N
}
bool StudentTextEditor::load(std::string file) {
    string x;
    ifstream infile(file);
    if(!infile)
    {
        return false;
    }
    
    if(!(m_size==1 && m_head->m_next->m_value.empty()))
        reset();
    if(m_head->m_next!=nullptr)
        doErase(m_head->m_next); //going to now add those new lines
    unsigned long r2;
    while(getline(infile,x))
    {
        r2=x.substr(0).find('\r'); //find has order N right? so this does not work then? ASK AT OH
        if(r2==string::npos)
        {
            r2=x.size();
        }
        string input=x.substr(0,r2);//that should be the correct length
//        if(m_size==1)
//        {
//            m_head->m_next->m_value=input; //this is for the first line
//        }
//        else
            insertAtBack(input);
        
    }
    m_currentRow=0;
    m_currentCol=0;
    m_currentRowPtr=m_head->m_next;
    return true;
    //THE ORDER HERE IS POTENTIALLY MESSED UP- THE WHILE LOOP SHOULD BE ORDER N, BUT THE FIND FUNCTION GOES THROUGH EACH CHAR SO IS ORDER L
    //this is corrected in the spec
}

bool StudentTextEditor::save(std::string file) {
    ofstream outfile(file);
    if(!outfile)
    {
        return false;
    }
    for(Node* p = m_head->m_next; p != m_head; p = p->m_next)
    {
            outfile<<p->m_value<<'\n';
    }
    return true;
    //THE FOR LOOP HAS THE ORDER M
}

void StudentTextEditor::reset() {
    //this will be more similar to the constructor rather than the destructor
    while (m_head->m_next != m_head)
        doErase(m_head->m_next);

      //don't delete the head
    getUndo()->clear();
    m_currentRow=0;
    m_currentCol=0;
    insertAtBack("");
    m_currentRowPtr=m_head->m_next; //back to having the empty line
    //THE WHILE LOOP HAS THE ORDER N
}

void StudentTextEditor::move(Dir dir) {
    switch(dir)
    {
        case Dir::UP:
        {
            if(m_currentRow!=0)
            {
                if(m_currentRowPtr->m_prev->m_value.size()<m_currentCol)
                    m_currentCol=m_currentRowPtr->m_prev->m_value.size(); //I think this is how this should work? check sample
                m_currentRow--;
                m_currentRowPtr=m_currentRowPtr->m_prev;
            }
        }
            break;
        case(Dir::DOWN):
        {
            if(m_currentRowPtr->m_next!=m_head)
            {
                if(m_currentRowPtr->m_next->m_value.size()<m_currentCol)
                    m_currentCol=m_currentRowPtr->m_next->m_value.size(); //I think this is how this should work? check sample
                m_currentRow++;
                m_currentRowPtr=m_currentRowPtr->m_next;
            }
        }
            break;
        case(Dir::LEFT):
        {
            if(m_currentCol==0 && m_currentRow!=0)
            {
                m_currentRow--;
                m_currentRowPtr=m_currentRowPtr->m_prev;
                m_currentCol=m_currentRowPtr->m_value.size();
            }
            else if(m_currentCol!=0)
            {
                m_currentCol--;
            }
        }
            break;
        case(Dir::RIGHT):
        {
            if(m_currentRowPtr->m_next!=m_head && m_currentCol==m_currentRowPtr->m_value.size())
            {
                m_currentRow++;
                m_currentRowPtr=m_currentRowPtr->m_next;
                m_currentCol=0;
            }
            else if(m_currentCol!=m_currentRowPtr->m_value.size())
            {
                m_currentCol++;
            }
        }
            break;
        case(Dir::HOME):
        {
            m_currentCol=0;
        }
            break;
        case(Dir::END):
        {
            m_currentCol=m_currentRowPtr->m_value.size();
        }
            break;
    }
}

void StudentTextEditor::del() {
    if(m_currentCol==m_currentRowPtr->m_value.size() && m_currentRowPtr->m_next!=m_head)//if it's one past the last and it's not the last line
    {
        getUndo()->submit(Undo::JOIN, m_currentRow, m_currentCol);
        m_currentRowPtr->m_value+=m_currentRowPtr->m_next->m_value;
        doErase(m_currentRowPtr->m_next);
        //all the location stats would stay the same
    }
    else if(m_currentCol<m_currentRowPtr->m_value.size()) //again, if it's not the last
    {
        getUndo()->submit(Undo::DELETE, m_currentRow, m_currentCol,m_currentRowPtr->m_value[m_currentCol]);
        m_currentRowPtr->m_value.erase(m_currentCol,1);
    }
    //THE += IS THE ORDER L1 + L2
    //THE ERASE FUNCTION IS THE ORDER L
    //ASK IN OH!
}

void StudentTextEditor::backspace() {
    if(m_currentCol==0 && m_currentRowPtr->m_prev!=m_head) //it is not the first
    {
        int oSize=m_currentRowPtr->m_prev->m_value.size();
        m_currentRowPtr->m_prev->m_value+=m_currentRowPtr->m_value;
        Node* temp = m_currentRowPtr->m_prev;
        doErase(m_currentRowPtr);
        m_currentRowPtr=temp;
        m_currentRow--;
        m_currentCol=oSize;
        getUndo()->submit(Undo::JOIN, m_currentRow, oSize);
    }
    else if(m_currentCol!=0)
    {
        getUndo()->submit(Undo::DELETE, m_currentRow, m_currentCol-1,m_currentRowPtr->m_value[m_currentCol-1]);
        m_currentRowPtr->m_value.erase(m_currentCol-1,1);
        m_currentCol--;
    }
    //THE ERASE FUNCTION IS THE ORDER L
    //THE += IS THE ORDER L1 + L2
}

void StudentTextEditor::insert(char ch) {
    string x;
    if(ch==9)//if it's tab
    {
        x= "    ";
        for(int i=0; i<4;i++)
            getUndo()->submit(Undo::INSERT, m_currentRow, m_currentCol+i,' ');
    }
    else
    {
        x.push_back(ch);
        getUndo()->submit(Undo::INSERT, m_currentRow, m_currentCol,ch);
    }
    m_currentRowPtr->m_value.insert(m_currentCol,x);
    m_currentCol+=x.size();
    //the insert function is the order L ?
    //THE INSERTION FUNCTION IS THE ORDER L
}

void StudentTextEditor::enter() {
    getUndo()->submit(Undo::SPLIT, m_currentRow, m_currentCol);
    string x = m_currentRowPtr->m_value.substr(m_currentCol,m_currentRowPtr->m_value.size()-m_currentCol);
    m_currentRowPtr->m_value.erase(m_currentCol,m_currentRowPtr->m_value.size());
    //can't use the insert At back here
    Node* insert = new Node;
    insert->m_value=x;
    insert->m_next=m_currentRowPtr->m_next;
    insert->m_prev=m_currentRowPtr;
    m_currentRowPtr->m_next->m_prev=insert;
    m_currentRowPtr->m_next=insert;
    m_currentCol=0;
    m_currentRow++;
    m_currentRowPtr=insert;
    m_size++;
    //SUBSTR AND ERASE ARE BOTH THE ORDER L
}

void StudentTextEditor::getPos(int& row, int& col) const {
    row=m_currentRow;
    col=m_currentCol;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    if(startRow<0 || numRows<0 || startRow>m_size)
        return -1;
    Node* original = m_currentRowPtr;
    int originalRowCount=m_currentRow;
    while(!lines.empty())
    {
        lines.pop_back();
    }
    while(originalRowCount!=startRow)
    {
        if(originalRowCount>startRow)
        {
            originalRowCount--;
            original=original->m_prev;
        }
        else
        {
            originalRowCount++;
            original=original->m_next;
        }
    }
    int i=0;
    for(i=0; original!=m_head && i<numRows;)
    {
        
        lines.push_back(original->m_value); //here copying over this value is order L right? ask in OH?
        original=original->m_next;
        i++;
    }
    return i;
    //CHECK THE ORDER FOR THIS ONE
}

void StudentTextEditor::undo() {
    int row;
    int col;
    int count;
    string text;
    int x = getUndo()->get(row, col, count, text);
    switch(x)
    {
        case(0):
            break;
        case(1):
        {
            while(m_currentRow!=row)
            {
                if(m_currentRow>row)
                {
                    m_currentRow--;
                    m_currentRowPtr=m_currentRowPtr->m_prev;
                }
                else
                {
                    m_currentRow++;
                    m_currentRowPtr=m_currentRowPtr->m_next;
                }
            }
            m_currentCol=col;
            m_currentRowPtr->m_value.insert(m_currentCol,text);
            m_currentCol+=text.size();
        }
            break;
        case(2):
        {
            while(m_currentRow!=row)
            {
                if(m_currentRow>row)
                {
                    m_currentRow--;
                    m_currentRowPtr=m_currentRowPtr->m_prev;
                }
                else
                {
                    m_currentRow++;
                    m_currentRowPtr=m_currentRowPtr->m_next;
                }
            }
            m_currentCol=col;
            string x = m_currentRowPtr->m_value.substr(m_currentCol,m_currentRowPtr->m_value.size()-m_currentCol);
            m_currentRowPtr->m_value.erase(m_currentCol,m_currentRowPtr->m_value.size());
            //can't use the insert At back here
            Node* insert = new Node();
            insert->m_value=x;
            insert->m_next=m_currentRowPtr->m_next;
            insert->m_prev=m_currentRowPtr;
            m_currentRowPtr->m_next->m_prev=insert;
            m_currentRowPtr->m_next=insert;
            m_size++;
        }
            break;
        case(3):
        {
            while(m_currentRow!=row)
            {
                if(m_currentRow>row)
                {
                    m_currentRow--;
                    m_currentRowPtr=m_currentRowPtr->m_prev;
                }
                else
                {
                    m_currentRow++;
                    m_currentRowPtr=m_currentRowPtr->m_next;
                }
            }
            m_currentCol=col-count+1;
            m_currentRowPtr->m_value.erase(m_currentCol,count);
        }
            break;
        case(4):
        {
            while(m_currentRow!=row)
            {
                if(m_currentRow>row)
                {
                    m_currentRow--;
                    m_currentRowPtr=m_currentRowPtr->m_prev;
                }
                else
                {
                    m_currentRow++;
                    m_currentRowPtr=m_currentRowPtr->m_next;
                }
            }
            m_currentCol=col;
            m_currentRowPtr->m_value+=m_currentRowPtr->m_next->m_value;
            doErase(m_currentRowPtr->m_next);
        }
            break;
    }
    
}
StudentTextEditor::Node* StudentTextEditor::doErase(Node* p)
{
      // Save pointer to p's successor

    Node* pnext = p->m_next;

      // Unlink p from the list and destroy it

    p->m_prev->m_next = p->m_next;
    p->m_next->m_prev = p->m_prev;
    delete p;

    m_size--;

    return pnext;
}
void StudentTextEditor::insertAtBack(string y)
{
    Node* x = new Node;
    x->m_value=y;
    x->m_next=m_head;
    x->m_prev=m_head->m_prev;
    m_head->m_prev->m_next=x;
    m_head->m_prev=x;
    m_size++;
}

