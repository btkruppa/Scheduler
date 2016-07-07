/***************************************************
    file:     Scheduler.h

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              CPU scheduler.

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/
#include "Analyzer.cpp"


class Scheduler
{
    private:
        int         current_time;
        int         oldest_task_pid;
        int         schedule_method;
        int         task_num;
        void        UpdateWaitStates();


        // First Come First Serve
        void        FCFS();
        int         FindOldestReadyTask();

        // Round Robin
        void        RR();
        int         current_serviced_amount;
        int         GetNextTaskRR();
        int         NumAvailableCPUS();

        // Shortest Remaining Time
        void        SRT();
        int         shortest_task_pid;
        int         FindShortestTask();

        // Highest Response Ratio Next
        void            HRRN();
        int             GetIndexHRR();
        float           TaskResponseRatio(int);

    public:
        Scheduler();
        Analyzer     analysis_tool;              // Object for analyzing performance
        ProcessTable p_table;                        // Process Control Block
        void         SetSchedulingMethod(int);   // Determine Scheduling Mode
        void         PassProcessesToCPU(int);    // Process the tasks
        float        MeanTurnaroundTime();       // Out mean turnaround time
        float        MeanResponseRatio();        // Output mean response ratio
         void        OutputAnalysis();
};
