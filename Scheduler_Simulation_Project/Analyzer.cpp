/***************************************************
    file:     Analyzer.h

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to analyze the
              scheduler's performance

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include "Analyzer.h"

/***************************************************
    function: OutputAnalysis

    purpose:  Output each of the metric values
              used to assess the scheduling of
              the process table.
***************************************************/
void Analyzer::OutputAnalysis(ProcessTable p_table)
{   cout << "T_t, R_t, W_t, R_r, Throughput = "
         << MeanTurnaroundTime(p_table)   <<", "
         << MeanResponseTime(p_table)   <<", "
         << MeanWaitTime(p_table) << ", "
         << MeanResponseRatio(p_table) <<", "
         << MeanThroughPut(p_table)
         << endl;
}

/***************************************************
    function: MeanTurnaroundTime

    purpose:  Output mean time between when a
              task arrives and when it is completed.
***************************************************/
float Analyzer::MeanTurnaroundTime(ProcessTable p_table)
{
    float sum = 0;
    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state != complete)
        {   cout << "\nERROR: Not all tasks complete for turnaround time computation.\n";
            return -1;
        }
        else
        {   float end   = p_table.tasks[i].completion_time;
            float start = p_table.tasks[i].process.arrival_time;
            sum += end - start;
        }
    return sum / float(p_table.tasks.size());
}

/***************************************************
    function: MeanResponseRatio

    purpose:  Output mean of turnaround time over
              the time required to be serviced.
***************************************************/
float Analyzer::MeanResponseRatio(ProcessTable p_table)
{
    float sum = 0;
    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state != complete)
        {   cout << "\nERROR: Not all tasks complete for turnaround time computation.\n";
            return -1;
        }
        else
        {   float end   = p_table.tasks[i].completion_time;
            float start = p_table.tasks[i].process.arrival_time;

            sum += (end - start)/float(p_table.tasks[i].process.TotalServiceTime());
        }
    return sum / float(p_table.tasks.size());
}


/***************************************************
    function: MeanWaitTime

    purpose:  Output mean time spent waiting by
              each task, i.e., find the difference
              between the mean turnaround time
              and mean service time.
***************************************************/
float Analyzer::MeanWaitTime(ProcessTable p_table)
{
    float sum = 0;
    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state != complete)
        {   cout << "\nERROR: Not all tasks complete for turnaround time computation.\n";
            return -1;
        }
        else
        {   float end   = p_table.tasks[i].completion_time;
            float start = p_table.tasks[i].process.arrival_time;

            sum += (end - start) - float(p_table.tasks[i].process.TotalServiceTime());
        }
    return sum / float(p_table.tasks.size());
}

/***************************************************
    function: MeanResponseTime

    purpose:  Output mean time spent waiting by
              each task, i.e., find the difference
              between the mean turnaround time
              and mean service time.
***************************************************/
float Analyzer::MeanResponseTime(ProcessTable p_table)
{
    float sum = 0;
    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state != complete)
        {   cout << "\nERROR: Not all tasks complete for turnaround time computation.\n";
            return -1;
        }
        else
        {
            sum += p_table.tasks[i].process.response_time;
        }
    return sum / float(p_table.tasks.size());
}



/***************************************************
    function: MeanThroughPut

    purpose:  Output the number of tasks completed
              per cycle of CPU time.
***************************************************/
float Analyzer::MeanThroughPut(ProcessTable p_table)
{   return number_processes / float(p_table.completion_time);
}
