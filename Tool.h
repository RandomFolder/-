#ifndef TOOL_H
#define TOOL_H
#include <QMouseEvent>
#include "Graph.h"

class Tool
{
public:
    virtual void do_stuff(QMouseEvent * evt, Graph & gr)=0;
};

#endif // TOOL_H
