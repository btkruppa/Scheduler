/***************************************************
    file:     PCB.cpp

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              Process Control Block (PCB) for an
              operating system, which contains
              a process and its associated context

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include "PCB.h"

PCB::PCB(){state = ready;}

/***************************************************
    function: UpdateState

    purpose:  Look at the amount of time serviced
              and determine whether the process
              has been completed, or whether it
              should be in a ready state or waiting
              state.

   commment:  The waiting state is used when there
              is an IO portion of the process that
              requries CPU time in addition to its
              CPU processing time.
***************************************************/
void PCB::UpdateState()
{   if(process.Complete())  // Determine whether process has been completed
    {   state = complete;   // Update state
        return;
    }

    if(current_method == rr)
    {
        if(state == running)
        {   q_time++;
            if(q_time >= q){
                state = blocked;
            }
        }

        if(state == running)
            return;
    }

    int counter = 0, wait_state = 0;

    // Increment counter until we find where in the cpu_io_times vector we currently are
    // with respect to time serviced. If 'i%2' is even, then we are in a state where the
    // task should be processes by the CPU. Otherwise, it is waiting for IO input.
    for(int i = 0; i < process.cpu_io_times.size() && process.time_serviced >= counter; i++)
    {   counter += process.cpu_io_times[i];
        wait_state = (i)%2;
    }

    if(wait_state%2 == 0){
        if(current_method == rr)
            state = blocked;
        else
            state = ready;
    }
    else
        state = waiting;
}

/***************************************************
    function: IncrementWaitTime

    purpose:  If a process is currently in a wait
              state, then increment the amount of
              time it has been serviced.

   commment:  This is used by the Scheduler when
              another process is being used and
              this one is waiting for an IO input,
              which means at each clock cycle the
              time serviced should be incremented.
***************************************************/
void PCB::IncrementWaitTime()
{   if(state == running || state == complete || state == blocked)
        return;
    if(state == waiting)
        process.time_serviced++;
    UpdateState();
}
