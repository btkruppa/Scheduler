/***************************************************
    file:     Process.h

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              individual processs, referred to
              as tasks in the PCB.

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

class Process
{   public:
        Process();
        bool        Complete();
        int         arrival_time = 0;
        int         blocked_time = 0;
        int         pid;
        int         priority;       // process priority 0 = high, 3 = low
        int         time_serviced;
        int         response_time = 0;
        vector<int> cpu_io_times;
        int         TotalServiceTime();
};


