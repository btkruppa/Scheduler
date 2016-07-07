/***************************************************
    file:     ProcessTable.h

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              Process Table for an operating system.

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include "PCB.cpp"

class ProcessTable
{   public:
        vector<PCB>     tasks;                      // Array of all the tasks to complete
        //Process*        link;
        void            AddTasks(vector<Process>);  // Add new tasks to PCB
        bool            AllTasksComplete();         // Indicate whether tasks are complete
        int             completion_time;
};
