/***************************************************
    file:     PCB.h

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              Process Control Block (PCB) for an
              operating system, which contains
              a process and its associated context

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include "Process.cpp"

#define     running         0       // Currently being used by CPU
#define     waiting         1       // Waiting for IO input
#define     ready           2       // Queued and ready to be processed by CPU
#define     complete        3       // Processing time complete
#define     blocked         4       // Used for preventing execution in Round Robin

class PCB
{   public:
        PCB();
        Process process;
        int     state;                  // State of process: running, waiting, ready, complete, blocked
        int     q_time;                 // Amount of time serviced in current round robin quantum
        int     completion_time;        // Time at which process was completed
        void    IncrementWaitTime();    // If in wait state, this will increment the time serviced
        void    UpdateState();          // Use time_serviced to update state to complete, waiting, or ready
};
