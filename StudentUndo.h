#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
#include <iostream>
class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    ~StudentUndo();
    struct undoes
    {
        Action m_Action;
        int m_row;
        int m_col;
        std::string m_string;
    };
    
    std::stack<undoes> m_Undoes;
};

#endif // STUDENTUNDO_H_
