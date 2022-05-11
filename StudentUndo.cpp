#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}
StudentUndo::~StudentUndo()
{
    clear();
}
void StudentUndo::submit(const Action action, int row, int col, char ch) {
    if(action==3 && !m_Undoes.empty() && m_Undoes.top().m_Action==1 && m_Undoes.top().m_row==row && m_Undoes.top().m_col==col)
    {
        m_Undoes.top().m_string+=ch;
    }
    else if(action==3 && !m_Undoes.empty() && m_Undoes.top().m_Action==1 && m_Undoes.top().m_row==row && m_Undoes.top().m_col==col+1) //couldn't this also just be using delete and then moving back one? check sample!!
    {
        m_Undoes.top().m_string+=ch;
        m_Undoes.top().m_col--;
    }
    else if(action==1 &&!m_Undoes.empty() && m_Undoes.top().m_Action==3 && m_Undoes.top().m_row==row && m_Undoes.top().m_col==col-1) 
    {
        m_Undoes.top().m_string+=ch;
        m_Undoes.top().m_col++;
    }
    else
    {
        undoes x;
        if(action==1)
            x.m_Action=DELETE;
        if(action==2)
            x.m_Action=JOIN;
        if(action==3)
            x.m_Action=INSERT;
        if(action==4)
            x.m_Action=SPLIT;
        x.m_row=row;
        x.m_col=col;
        x.m_string+=ch;
        m_Undoes.push(x);
    }
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    if(m_Undoes.empty())
        return ERROR;
    row=m_Undoes.top().m_row;
    col=m_Undoes.top().m_col;
    if(m_Undoes.top().m_Action==3)
        count=m_Undoes.top().m_string.size();
    else
        count=1;
    text=m_Undoes.top().m_string;
    Action* temp=&m_Undoes.top().m_Action;
    m_Undoes.pop();
    return *temp;
}

void StudentUndo::clear() {
	while(!m_Undoes.empty())
    {
        m_Undoes.pop();
        //not rly allocating any memory so the default destructor should work
    }
}
