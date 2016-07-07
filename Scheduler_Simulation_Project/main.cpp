/***************************************************
    file:     main.cpp

    class:    CPTR 352 - OS Design

    purpose:  Run the high level code for the CPU
              scheduler simulation.

    creators: Howard Heaton & Blake Kruppa

    date:     June 2016
***************************************************/

#include <iostream>
#include <vector>
#include <windows.h>
#include <climits>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

#define     output_scheduler_info   FALSE
#define     test_all_methods        TRUE
/**********************************************
    Define the scheduling modes
**********************************************/
#define     fcfs            1       // first-come, first-serve
#define     rr              2       // round robin
#define     srt             3       // shortest remaining time
#define     hrrn            4       // highest response ratio next
int         current_method;


/**********************************************
    Define combinations of schedule modes
**********************************************/
#define     method_a        1       // RR(q=1), FCFS, HRRN
#define     method_b        2       // RR(q=4), FCFS, HRRN
#define     method_c        3       // RR(q=8), FCFS, SRT
#define     method_d        4       // RR(q=4), SRT, HRRN
#define     method_fcfs     5       // FCFS
#define     method_rr       6       // RR
#define     method_srt      7       // SRT
#define     method_hrrn     8       // HRRN

int         method_selection = method_rr;

/**********************************************
    Define other execution parameters
**********************************************/
#define     t_final         15000       // Length to run CPU in terms of milliseconds.
#define     num_cpus        2
int         q  = 1;                     // time quantum for round robin
/**********************************************
    Define process simulation file parameters
**********************************************/
#define     max_cpu_io_time     10       // the maximum cpu burst or io time generated
#define     max_cpu_bursts      4      // the maximum number of cpu bursts, note each burst will have an i/o wait between them
#define     generate_files      FALSE   // if true it will generate files, if false will run the scheduler
#define     number_processes    5000      // the number of processes to be generated.
#define     process_file        "process_file.csv"  //the process file that will be read or written to
#define     max_arrival_spacing    (max_cpu_io_time*max_cpu_bursts)/3
int max_arrival_time = 0;

#include    "Scheduler.cpp"

vector<Process> process_collection;
vector<Process> GetNewTasks(int);
vector<Process> ImportProcesses();
vector<int>     MethodOrder(int);
void            GenerateFiles();


/***************************************************
    function: main

    purpose:  Run the high level for the CPU
              scheduler simulation.
***************************************************/
int main()
{
    vector<int> method_order;
    bool only_once;
    if(test_all_methods){
        method_selection = 1;
    }
    else
        only_once = TRUE;
    while(test_all_methods || only_once){

    method_order = MethodOrder(method_selection);

    if(method_order[0] == 0)
        break;


    if(!generate_files){
    /*** Initializations ***/
    Scheduler       cpu_scheduler;              // Object representing the CPU scheduler
    vector<Process> new_tasks;                  // Array of processes to be added to PCB
    process_collection = ImportProcesses();     // Import all processes to be run from .txt file
    cpu_scheduler.SetSchedulingMethod(method_order[0]);    // Choose CPU scheduling method
    int method_counter = 0;
    current_method = method_order[0];

    int num_methods = method_order.size();

    /** Processing Simulation **/
        int t = 0;
        int method_count = 0;
        //note that this loop depends on the last possible arrival time of processes being the minimum amount of loops
        while(t<max_arrival_time || (t >= max_arrival_time && !cpu_scheduler.p_table.AllTasksComplete()))           // Take successive time steps until reach final time
        {   // Set CPU Scheduling method
            // In SetMethod account for transitions (e.g., set blocked states to ready)
            new_tasks = GetNewTasks(t);             // Identify processes that arrive at time 't'
            cpu_scheduler.p_table.AddTasks(new_tasks);  // Add new processes to the PCB
            cpu_scheduler.PassProcessesToCPU(t);    // Tell CPU which tasks to process
            t++;

            if(method_counter%1000 == 0){
                if(method_counter/1000 >= method_order.size())
                    method_counter = 0;
                current_method = method_order[method_counter/1000];
                cpu_scheduler.SetSchedulingMethod(current_method);

            }
            method_counter ++;
        }

        if(cpu_scheduler.p_table.AllTasksComplete())    // Output whether all tasks in PCB have been completed
            cout << "All processes complete!\n";
        else
            cout << "There's still remaining tasks in the PCB.\n";

        cout << "Scheduling method = " << method_selection << endl;
        cpu_scheduler.OutputAnalysis();
        cout << "end time " << t << endl;
    }
    else{
        GenerateFiles();
        cout << "File Generated";
    }
    method_selection ++;
    only_once = FALSE;
    }

    return 0;
}

/***************************************************
    function: GetNewTasks

    purpose:  Determine which tasks have arrived to
              be processed at time 't'.

    input:    time 't'

    output:   vector containing new tasks
***************************************************/
vector<Process> GetNewTasks(int t)
{   vector<Process>     new_tasks;

    for(int i = 0; i < process_collection.size(); i++)
        if(process_collection[i].arrival_time == t)
            new_tasks.push_back(process_collection[i]);

    return new_tasks;
}

vector<int> MethodOrder(int selection){
    vector<int> method_order;
    switch(selection)
    {
        case method_a: method_order.push_back(rr);
                q=1;
                method_order.push_back(fcfs);
                method_order.push_back(hrrn);
                break;
        case method_b: method_order.push_back(rr);
                q=4;
                method_order.push_back(fcfs);
                method_order.push_back(hrrn);
                break;
        case method_c: method_order.push_back(rr);
                q = 8;
                method_order.push_back(fcfs);
                method_order.push_back(srt);
                break;
        case method_d: method_order.push_back(rr);
                q = 4;
                method_order.push_back(srt);
                method_order.push_back(hrrn);
                break;
        case method_fcfs: method_order.push_back(fcfs);
                break;
        case method_rr: method_order.push_back(rr);
                break;
        case method_srt: method_order.push_back(srt);
                break;
        case method_hrrn: method_order.push_back(hrrn);
                break;
        default: method_order.push_back(0);

    }
    return method_order;
}


/***************************************************
    function: GenerateFiles

    purpose:  Generate a .cvs file based on parameters
              defined at the top

    input:    none

    output:   creates a file
***************************************************/
void GenerateFiles(){
    ofstream processFile;
    processFile.open (process_file, ios::out | ios::ate | ios::app| ios::binary) ;

    srand(time(NULL));
    processFile << " ";
    int arrives = 0;
    for(int i = 1; i <= number_processes ; i++){
        processFile << "PID: " << i << "," << endl;
        processFile << "arrival time: " << arrives << "," << endl;
        arrives += rand()%max_arrival_spacing;

        int cpu_io_quantity = rand()%max_cpu_bursts*2 + 1; //randomize a number of cpu bursts, multiplies it by two and adds one so that this number is always odd making us have more cpu burst than i/o waits
        for(int j = 1; j <= cpu_io_quantity; j++){
            if(j%2 == 0){
                processFile << "i/o time: " << rand()%(max_cpu_io_time - 1) + 1 << "," << endl;
            }
            else{
                processFile << "cpu time: " << rand()%(max_cpu_io_time - 1) + 1 << "," << endl;
            }
        }
        processFile << "end process" << "," << endl;
    }
    processFile.close();
    return;
}

vector<Process> ImportProcesses()
{
    vector<Process> new_tasks;

    vector<int>    times;
    ifstream file ( process_file ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
    string value;
    Process a;
    a.priority     = 1;
    int pop_count = 0;
    while ( file.good() )
    {
         getline ( file, value, ',' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
         if(string(value).length() < 2){
            break;
         }
         else if(string(value,1,3) == "PID"){ //reading the PID for the process
            if(string(value).length() >=5 ){
                a.pid = atoi(string(value,5, value.length()-1).c_str());
                //cout << string( value, 1, value.length()-1 ) << endl; // display value removing the first 6 chars and the last character from it
            }
         }
         else if(string(value,1,12) == "arrival time"){ //reading the arrival time for process]
            //cout << string( value, 1, value.length()-1 ) << endl;
            a.arrival_time = atoi(string(value,15, value.length()-1).c_str());

         }
         else if(string(value,1,8) == "cpu time"){ //reading the cpu_io_times for the process]
            times.push_back(atoi(string(value,10, value.length()-1).c_str()));
            a.cpu_io_times = times;
            //cout << string( value, 1, value.length()-1 ) << endl; // display value removing the first and the last character from it
            pop_count ++;
         }
         else if(string(value,1,8) == "i/o time"){ //reading the cpu_io_times for the process
            times.push_back(atoi(string(value,10, value.length()-1).c_str()));
            a.cpu_io_times = times;
            //cout << string( value, 1, value.length()-1 ) << endl; // display value removing the first and the last character from it
            pop_count ++;
         }
         else if(string(value,1,3) == "end"){ //reading the cpu_io_times for the process
            new_tasks.push_back(a);
            for(int i = 1; i <= pop_count; i++){
                times.pop_back();
            }
            pop_count = 0;
         }


    }
    file.close();
    cout << "imported";



    /*Process a;
    a.priority     = 1;
    //vector<int>    times;

    a.pid          = 1;
    a.arrival_time = 0;
    times.push_back(3); //service time
    //times.push_back(5);
    //times.push_back(4);
    a.cpu_io_times = times;
    new_tasks.push_back(a);

    a.pid          = 2;
    a.arrival_time = 2;
    times.pop_back();
    //times.pop_back();
    //times.pop_back();
    times.push_back(6);
    a.cpu_io_times = times;
    new_tasks.push_back(a);

    a.pid          = 3;
    a.arrival_time = 4;
    times.pop_back();
    times.push_back(4);
    a.cpu_io_times = times;
    new_tasks.push_back(a);

    a.pid          = 4;
    a.arrival_time = 6;
    times.pop_back();
    times.push_back(5);
    a.cpu_io_times = times;
    new_tasks.push_back(a);

    a.pid          = 5;
    a.arrival_time = 8;
    times.pop_back();
    times.push_back(2);
    a.cpu_io_times = times;
    new_tasks.push_back(a);*/

    max_arrival_time = a.arrival_time;

    return new_tasks;
}
