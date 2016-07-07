/***************************************************
    file:     Scheduler.cpp

    class:    CPTR 352 - OS Design

    purpose:  Provide the class to represent the
              CPU scheduler.

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/
#include "Scheduler.h"

Scheduler::Scheduler()
{   schedule_method = fcfs;    // Initially assume first-come first-serve approach
    oldest_task_pid = -1;
    task_num        = -1;
    current_serviced_amount = 0;
}

/***************************************************
    function: SetSchedulingMethod

    purpose:  Identify which method to use when
              scheduling tasks to be processed by
              the CPU

    input:    choice of method
***************************************************/
void Scheduler::SetSchedulingMethod(int method)
{
    for(int i =0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state == blocked)
            p_table.tasks[i].state = ready;
    schedule_method = method;
}


/***************************************************
    function: PassProcessesToCPU

    purpose:  Schedule tasks to be processed by CPU
              according to chosen method.

    input:    current clock time
***************************************************/
void Scheduler::PassProcessesToCPU(int time)
{   current_time = time;

    if(p_table.AllTasksComplete())
    {
        if(output_scheduler_info)
                cout << "No remaining tasks to be processed.\n";
        return;
    }

    /// Identify running states
    switch(schedule_method)
    {   case fcfs:  FCFS(); break;
        case rr:    RR();   break;
        case srt:   SRT();  break;
        case hrrn:  HRRN(); break;
    }


    if(output_scheduler_info == TRUE){
        cout << "Clock Time = "<<current_time<<",  \tTasks: ";
    }
    /// Proces running states
    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state == running)
        {
            if(p_table.tasks[i].process.response_time == 0)
                p_table.tasks[i].process.response_time = current_time - p_table.tasks[i].process.arrival_time;

            if(output_scheduler_info == TRUE)
                cout << p_table.tasks[i].process.pid << ",\t";
            p_table.tasks[i].process.time_serviced += 1;    // Spend 1 clock cycle processing task
        }
    if(output_scheduler_info == TRUE)
        cout << endl;


    /// Update state of each task
    UpdateWaitStates();
    for(int i = 0; i < p_table.tasks.size(); i++)   // Update each running state
    {   if(p_table.tasks[i].state == running)
        {   p_table.tasks[i].UpdateState();         // Must come after 'UpdateWaitStates()'
            if(p_table.tasks[i].state == complete)
                p_table.tasks[i].completion_time = current_time+1;   // Store time at which task was completed
            if(current_method == rr){
                if(p_table.tasks[i].state == blocked || p_table.tasks[i].state == waiting)
                    p_table.tasks[i].process.blocked_time = current_time+1;
            }

        }
    }

}

/***************************************************
    function: MeanTurnAroundTime

    purpose:  Compute the mean turn around time
              for tasks processed by the CPU
***************************************************/
float Scheduler::MeanTurnaroundTime()
{   return analysis_tool.MeanTurnaroundTime(p_table);
}

/***************************************************
    function: MeanTurnAroundTime

    purpose:  Compute the mean ratio of the
              response time over service time for
              tasks processed by the CPU
***************************************************/
float Scheduler::MeanResponseRatio()
{   return analysis_tool.MeanResponseRatio(p_table);
}
/***************************************************
    function: UpdatWaitStates

    purpose:  Cycle through each of the wait states
              and update their wait times and,
              if necessary, change them to the
              ready state
***************************************************/
void Scheduler::UpdateWaitStates()
{   for(int i = 0; i < p_table.tasks.size(); i++)
        p_table.tasks[i].IncrementWaitTime();
}


/***************************************************
    function: FCFS

    purpose:  Process data from Process Table using
              first-comefirst-serve approach.
***************************************************/
void Scheduler::FCFS()
{   for(int i = 0; i < num_cpus; i++)
    {   task_num = FindOldestReadyTask();             // Find oldest task to process.
        if(task_num != -1)                            // If a task in Process Table is ready
            p_table.tasks[task_num].state = running;  // Set it in running state.
    }
}

/***************************************************
    function: FindOldestReadyTask

    purpose:  Identify the task that came first
              for use in the first-come first-serve
              scheduling scheme.

    output:   index in p_table of oldest task
***************************************************/
int Scheduler::FindOldestReadyTask()
{   for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state == ready)
            return i;

    return -1;
}

/***************************************************
    function: Round Robin

    purpose:  Process data from Process Table
              using the Round Robin approach.
***************************************************/
void Scheduler::RR()
{
    /// Identify running states
    while(NumAvailableCPUS() > 0 && GetNextTaskRR() != -1){
        task_num = GetNextTaskRR();
        p_table.tasks[task_num].state = running;  // Set it in running state.
        p_table.tasks[task_num].q_time = 0;
    }
}

/***************************************************
    function: NumAvailableCPUS

    purpose:  Identify difference between number of
              CPUs and number of tasks in running state.
***************************************************/
int Scheduler::NumAvailableCPUS()
{   int running_states = 0;
    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state == running)
            running_states++;

    return num_cpus - running_states;
}


/***************************************************
    function: Find the next task to service for RR

    purpose:  Identify the task should be serviced
              next in the RR scheduling scheme

    output:   PID of next task to be serviced
***************************************************/
int Scheduler::GetNextTaskRR()
{
    int task_num = -1;
    int start_time = INT_MAX;

    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state != complete && p_table.tasks[i].process.arrival_time <= start_time)
        {
            if(p_table.tasks[i].state == blocked && p_table.tasks[i].process.blocked_time < start_time){
                start_time = p_table.tasks[i].process.blocked_time;
                task_num   = i;
            }
            else if(p_table.tasks[i].state == ready){
                start_time = p_table.tasks[i].process.arrival_time;
                task_num   = i;
            }
        }
    return task_num;
}

/***************************************************
    function: SRT

    purpose:  Process data from PCB using shortest
              remaining time approach.
***************************************************/
void Scheduler::SRT()
{
    for(int i = 0; i < num_cpus; i++)
    {   task_num = FindShortestTask();             // Find oldest task to process.
        if(task_num != -1)                            // If a task in Process Table is ready
            p_table.tasks[task_num].state = running;  // Set it in running state.
    }
}




/***************************************************
    function: FindShortestTask

    purpose:  Identify the task with the shortest remaining time
              for use in the shortest remaining time
              scheduling scheme.

    output:   task number of shortest task
***************************************************/
int Scheduler::FindShortestTask()
{
    int pid;
    int shortest_time = INT_MAX;
    task_num = -1;

    for(int i = 0; i < p_table.tasks.size(); i++)
        if(p_table.tasks[i].state == ready && (p_table.tasks[i].process.TotalServiceTime() - p_table.tasks[i].process.time_serviced) < shortest_time)
        {   shortest_time = p_table.tasks[i].process.TotalServiceTime() - p_table.tasks[i].process.time_serviced;
            task_num   = i;
        }

    return task_num;
}

/***************************************************
    function: HRRN

    purpose:  Identify the running states by
              identifying the tasks with the high-
              est response ratio next.
***************************************************/
void Scheduler::HRRN()
{   for(int i = 0; i < num_cpus; i++)                   // for each CPU
    {   int           index_hrr        = GetIndexHRR(); // get index of HRR among ready tasks
        p_table.tasks[index_hrr].state = running;       // set that task to running
    }
}

/***************************************************
    function: TaskResponseRatio

    purpose:  Compute the response ratio of the
              'i'-th task in the Process Table.
***************************************************/
float Scheduler::TaskResponseRatio(int i)
{
    if(p_table.tasks[i].state != ready)     // Only let ready tasks give nonzero response ratio
        return 0;

    Process p = p_table.tasks[i].process;

    float wait_time      = current_time         - p.arrival_time - p.time_serviced;
    float time_remaining = p.TotalServiceTime() - p.time_serviced;
    return (time_remaining + wait_time) / time_remaining;   // return formula output for wait time
}

/***************************************************
    function: GetIndexHRR

    purpose:  Find the index in the Process Table
              of the task with the highest response
              ratio.
***************************************************/
int Scheduler::GetIndexHRR()
{
    vector<float> response_ratios;                      // array containing all response ratios
    for(int i = 0; i < p_table.tasks.size(); i++)
        response_ratios.push_back(TaskResponseRatio(i));

    int index_hrrn = INT_MAX;
    int max_rr     = INT_MIN;
    for(int i = 0; i < response_ratios.size(); i++)
        if(response_ratios[i] > max_rr)
        {   index_hrrn = i;
            max_rr     = response_ratios[i];
        }

    // The following snippet sets index_hrrn to the index
    // of the task the oldest task in the PCB that has the
    // highest response ratio. This enables a secondary
    // criterion of first-come first-serve, in case there
    // are multiple tasks with the same response ratio.
    index_hrrn = 0;
    while(response_ratios[index_hrrn] < max_rr)
        index_hrrn++;

    return index_hrrn;
}

/***************************************************
    function: OutputAnalysis

    purpose:  Output each of the metric values
              used to assess the scheduling of
              the process table.
***************************************************/
void Scheduler::OutputAnalysis()
{
    p_table.completion_time = current_time;
    analysis_tool.OutputAnalysis(p_table);
}
