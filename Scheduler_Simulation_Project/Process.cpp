/***************************************************
    file:     Process.cpp

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              individual processs, referred to
              as tasks in the PCB.

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include "Process.h"

Process::Process(){ time_serviced = 0;}

int  Process::TotalServiceTime()
{   int total_service_time = 0;
    for(int i = 0; i < cpu_io_times.size(); i++){
        total_service_time += cpu_io_times[i];
    }
    return total_service_time;
}



bool Process::Complete()
{
    if(time_serviced < TotalServiceTime())
        return false;
    else
        return true;
}

