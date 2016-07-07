/***************************************************
    file:     ProcessTable.h

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              Process Table for an operating system.

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include "ProcessTable.h"

/***************************************************
    function: AddTasks

    purpose:  Add a group of new tasks to the table.

    comment:  This is used to update the table as
              new tasks are passed to the scheduler
              during simulation since not all of the
              tasks arrive at the initial time.
***************************************************/
void ProcessTable::AddTasks(vector<Process> new_tasks)
{
    PCB temp;
    temp.state                  = ready;    // Initialize newly added task to be ready for processing.
    temp.process.time_serviced  = 0;        // Initially, no time has been spent service process.

    for(int i = 0; i < new_tasks.size(); i++)
    {   temp.process = new_tasks[i];
        tasks.push_back(temp);
    }
}

/***************************************************
    function: AddTasks

    purpose:  Add a group of new tasks to the table.

    comment:  This is used to update the table as
              new tasks are passed to the scheduler
              during simulation since not all of the
              tasks arrive at the initial time.
***************************************************/
bool ProcessTable::AllTasksComplete()
{
    if(tasks.size() == 0)
        return true;

    for(int i = 0; i < tasks.size(); i++)
        if(tasks[i].state != complete)
            return false;
    return true;
}
