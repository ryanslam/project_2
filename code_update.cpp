#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <float.h>
// #include <bits/stdc++.h>
using namespace std;
 
 
//Global Variables
char next_process_name = 'A';
 
//done
//changed variables to float and added 0.0 to all initialized values !!!!!!!!!!!!!!!!!!!!!!!!!!!
class performance_metrics
{
  public:
  float response_time;
  float turnaround_time;
  float waiting_time;
  float total_quanta; 
  float start_time;
  float end_time;
  performance_metrics()
  {
      response_time = 0.0;
      turnaround_time = 0.0;
      waiting_time = 0.0;
      total_quanta = 1.0;
      start_time = 0.0;
      end_time = 0.0;
  }
};
 
//done
//changed the arrival time and service time as floats !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
class process
{
  public:
  float arrival_time;
  float service_time;
  float remaining_service_time;
  int priority;
  char process_name;
  int job_start_time;
  performance_metrics metrics;
  process()
  {
      arrival_time = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(99))); // will return number between 0 and 99
      service_time = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(10)));//will return number between 0 and 10
      remaining_service_time = service_time;
      priority = (rand() % 5);
      if (priority == 0)
          priority += 1;// priority between 1 .. 4
      process_name = next_process_name;
      next_process_name++;
  }
};
 
//done
bool processComparator(process p1, process p2)
{
  return (p1.arrival_time < p2.arrival_time);
}
 
//done
void createProcessList(vector<process> &process_list, int extra_processes = 0)
{
  process_list.clear();
  next_process_name = 'A';
 
  int processes_count = min(10 + extra_processes, 52); //generating 10 by default, adding extra processes if requested with the limit of maximum 52 processes.
 
  cout << "\npopulating new processes..." << endl;
 
  for(int i = 0; i < processes_count; i++)
  {
      if ( i == 26 )
      {
          next_process_name = 'a';
      }
      process p;
      process_list.push_back(p);
  }
  sort(process_list.begin(), process_list.end(), processComparator);
}
 
//done, but has problems with long floats
void printProcessList(vector<process> &process_list)
{
   cout << setprecision(3) << fixed;
   cout << "Processes Name  " << "Arrival Time   " << " Expected Run Time   " << "Priority" << endl;
   for(int process_index = 0; process_index<process_list.size(); ++process_index)
       cout << process_list[process_index].process_name << "\t\t"
           << process_list[process_index].arrival_time << "\t\t"
           << process_list[process_index].service_time << "\t\t    "
           << process_list[process_index].priority << endl;
}

//used for printing by start time to easily match it with time map
bool postProcessComparator(process p1, process p2)
{
  return (p1.metrics.start_time < p2.metrics.start_time);
}

//used to print the executed processes
void postPrintProcessList( vector<process> &process_list )
{
        //sort by start time for the exectued processes
        sort(process_list.begin(), process_list.end(), postProcessComparator);
        cout << "Process Name\t" << "Start Time\t" << "End Time\t" << "Response Time\t" 
                << "Service Time\t" << "Wait Time\t" << "Turnaround Time\t" << endl;
        for (int i = 0; i < process_list.size(); i++ )
        {
            if ( process_list[ i ].metrics.turnaround_time == 0)
                continue;
            
            cout << setprecision( 3 );
            cout << process_list[ i ].process_name << "\t\t";
            cout << process_list[ i ].metrics.start_time << "\t\t";
            cout << process_list[ i ].metrics.end_time << "\t\t";
            cout << process_list[ i ].metrics.response_time << "\t\t";
            cout << process_list[ i ].service_time << "\t\t";
            cout << process_list[ i ].metrics.waiting_time << "\t\t";
            cout << process_list[ i ].metrics.turnaround_time << endl;
        }
        //revert to arrival time sorting at the end of the function
        sort(process_list.begin(), process_list.end(), processComparator);

}

 
//done
void printTimeChart(vector<char> &time_chart)
{
  cout << "Time Chart : [ ";
  for(int chart_index = 0; chart_index<time_chart.size(); ++chart_index)
      cout << time_chart[chart_index];
  cout << " ]" << endl;
}
 
 
void calculateAndPrintPerformanceMetrics( vector<vector<process> > &workloads)
{
  // average metrics for all the workloads.
  float avg_turnaround_time = 0.0;
  float avg_response_time = 0.0;
  float avg_waiting_time = 0.0;
  float avg_throughput = 0.0;
 
 
  for(int workload_index = 0; workload_index < workloads.size(); ++workload_index)
  {
     
      //changed these variables to floats !!!!!!!!!!!!!!!!!!!!
      // for current workload
      float total_turnaround_time = 0.0;
      float total_response_time = 0.0;
      float total_waiting_time = 0.0;
 
 
      int processes_completed = 0;
      vector<process> &process_list = workloads[workload_index];
      for(int process_index = 0; process_index<process_list.size(); ++process_index)
      {
          if(process_list[process_index].metrics.turnaround_time == 0)
              continue;
          ++processes_completed;
          total_turnaround_time += process_list[process_index].metrics.turnaround_time;     
          total_waiting_time += process_list[process_index].metrics.waiting_time;
          total_response_time += process_list[process_index].metrics.response_time;
      }
 
 
   //Changed it so you do not multiply by 1.0 for each of the total times. !!!!!!!!!!!!!!!
      cout << "Performance metrics for workload " <<  workload_index+1 << " :" << endl;
      cout << "\tAverage Turnaround Time : " << total_turnaround_time/processes_completed << endl;
      avg_turnaround_time += total_turnaround_time/processes_completed;
      cout << "\tAverage Waiting Time : " << total_waiting_time/processes_completed << endl;
      avg_waiting_time += total_waiting_time/processes_completed;
      cout << "\tAverage Response Time : " << total_response_time/processes_completed << endl;
      avg_response_time += total_response_time/processes_completed;
      cout << "\tThroughput (processes per quanta): " << processes_completed/workloads[workload_index][0].metrics.total_quanta << endl;
      avg_throughput += processes_completed/workloads[workload_index][0].metrics.total_quanta;


  }
  cout << "Average Performance metrics for all workload :" << endl;
  cout << "Average Turnaround Time : " << avg_turnaround_time/workloads.size() << endl;
  cout << "Average Waiting Time : " << avg_waiting_time/workloads.size() << endl;
  cout << "Average Response Time : " << avg_response_time/workloads.size() << endl;
  cout << "Average Throughput : " << avg_throughput/workloads.size() << endl;

}
 
 
 
int selectProcessForHpf (vector<vector<process> > &priority_queue, int quanta, bool isPreemptive)
{
   if(!isPreemptive)
   {
       for(int queue_index = 0; queue_index < priority_queue.size();queue_index++)
       {
           if(priority_queue[queue_index].size() > 0 &&
               priority_queue[queue_index][0].remaining_service_time != priority_queue[queue_index][0].service_time)
               return queue_index;
       }      
   }
   for(int queue_index = 0; queue_index<priority_queue.size();queue_index++)
   {
       if( priority_queue[queue_index].size() > 0)
       {
           if( quanta>=100 &&
               priority_queue[queue_index][0].remaining_service_time == priority_queue[queue_index][0].service_time)
               {
                   priority_queue[queue_index].erase(priority_queue[queue_index].begin());
                   return selectProcessForHpf(priority_queue, quanta, isPreemptive);
               }
           return queue_index;
       }
   }
   return -1;
}
 
//Changed remaining_service_time from int to float
//done
bool highestPriorityFirst(vector<process> &process_list, vector<char> &time_chart, bool isPreemptive, vector<vector<process> > &workloads)
{
   int process_index = 0;
   float remaining_service_time = 0.0;
   int current_cpu_consecutive_idle_time = 0;
   int max_cpu_consecutive_idle_time = 0;
   //float quantas = 0.0;
   vector<vector<process> >priority_queue(4);
   vector<vector<process> > complete_process_list(4);
 
 
   for(int quanta=0; true ; ++quanta)
  {
      //quantas++;
      while(quanta<100 && process_index < process_list.size() && process_list[process_index].arrival_time <= quanta)
      {
          priority_queue[process_list[process_index].priority-1].push_back(process_list[process_index]);
          process_index++;
      }
 
      int priority_queue_index = selectProcessForHpf(priority_queue, quanta, isPreemptive);
      if(quanta >= 100)
      {
          if(  priority_queue[0].size() == 0 &&
               priority_queue[1].size() == 0 &&
               priority_queue[2].size() == 0 &&
               priority_queue[3].size() == 0 )
               break;
      }
      if(priority_queue_index == -1)
      {
          time_chart.push_back('-');
          ++current_cpu_consecutive_idle_time;
          max_cpu_consecutive_idle_time = max(max_cpu_consecutive_idle_time, current_cpu_consecutive_idle_time);
      }
      else
      {
        if( priority_queue[priority_queue_index][0].remaining_service_time ==  priority_queue[priority_queue_index][0].service_time)
            priority_queue[priority_queue_index][0].metrics.response_time = quanta -  priority_queue[priority_queue_index][0].arrival_time;
            
        current_cpu_consecutive_idle_time = 0;
        priority_queue[priority_queue_index][0].remaining_service_time = max((float)0.0,priority_queue[priority_queue_index][0].remaining_service_time-1);
        time_chart.push_back( priority_queue[priority_queue_index][0].process_name);
        
        if(priority_queue[priority_queue_index][0].remaining_service_time == 0)
        {
            // updating performance metrics
            priority_queue[priority_queue_index][0].metrics.turnaround_time = quanta+1 - priority_queue[priority_queue_index][0].arrival_time;
            priority_queue[priority_queue_index][0].metrics.waiting_time = priority_queue[priority_queue_index][0].metrics.turnaround_time - priority_queue[priority_queue_index][0].service_time;
            
            complete_process_list[priority_queue_index].push_back( priority_queue[priority_queue_index][0]);
            
            priority_queue[priority_queue_index].erase(priority_queue[priority_queue_index].begin());              
        }
        else if(isPreemptive)
        {
            priority_queue[priority_queue_index].push_back(priority_queue[priority_queue_index][0]);
            priority_queue[priority_queue_index].erase(priority_queue[priority_queue_index].begin());
        }
         
    }
    if(max_cpu_consecutive_idle_time < 2){
        workloads = complete_process_list;
    }
  }
  //process_list[ 0 ].metrics.total_quanta = quantas;
  return max_cpu_consecutive_idle_time < 2 ? true : false;
}  
 
// Round Robin Scheduling Algorithm Implementation.
bool runRoundRobin(vector<process> &process_list, vector<char> &time_chart, vector<vector<process> > &workloads){
    int timeQuanta = 0;
    int process_index = 0;
    int current_cpu_consecutive_idle_time = 0;
    int max_cpu_consecutive_idle_time = 0;
    //float quantas = 0.0;
    // Initialize queue populated with sorted process list.
    queue<process> requestQueue;
    vector<process> metric_list;
    // Run round robin until all processes are completed or the quanta is greater than or equal 100.
    while(timeQuanta < 100 || !requestQueue.empty()){
        //quantas++;
        while (process_index < process_list.size() && process_list[process_index].arrival_time <= timeQuanta && timeQuanta < 100){
            requestQueue.push(process_list[process_index]);
            (requestQueue.front()).metrics.response_time = timeQuanta - (requestQueue.front()).arrival_time;
            process_index++;
        }
        // If there are still processes, run the processes and update the values.
        if(!requestQueue.empty()){
            if((requestQueue.front()).remaining_service_time == (requestQueue.front()).service_time){
                (requestQueue.front()).job_start_time = timeQuanta;    
                (requestQueue.front()).metrics.start_time = timeQuanta;  
            }
        }
        
        // Reduce the service time by 1 quanta
        if(!requestQueue.empty()){
            (requestQueue.front()).remaining_service_time = max((float)0, (requestQueue.front()).remaining_service_time-1);
            time_chart.push_back(requestQueue.front().process_name);
        
        
            // Check if the process needs to be placed at the back of the queue.
            if((requestQueue.front()).remaining_service_time > 0){
                // If it hasn't been completed, push the process back into the queue.          
                process tmp = requestQueue.front();
                requestQueue.pop();
                requestQueue.push(tmp);
                // Reset CPU consecutive idle time.
                current_cpu_consecutive_idle_time = 0;
            }
            else{
                // Update the turnaround and waiting time.
                //   cout << (requestQueue.front().job_start_time) << endl;
                (requestQueue.front()).metrics.total_quanta = timeQuanta;
                (requestQueue.front()).metrics.turnaround_time = timeQuanta - (requestQueue.front().job_start_time);
                (requestQueue.front()).metrics.waiting_time = (requestQueue.front()).metrics.turnaround_time - (requestQueue.front()).service_time;
                (requestQueue.front()).metrics.end_time = timeQuanta;
                metric_list.push_back(requestQueue.front());
                requestQueue.pop();
            }
        }
        else{
            time_chart.push_back('-');
            ++current_cpu_consecutive_idle_time;
            max_cpu_consecutive_idle_time = max(max_cpu_consecutive_idle_time, current_cpu_consecutive_idle_time);
        }
        timeQuanta++;
    }
    
    
    if(max_cpu_consecutive_idle_time < 2){
        workloads.push_back(metric_list);
    }
    // Returns whether cpu consecutive idle time exceeds 2.
    
    return max_cpu_consecutive_idle_time < 2 ? true : false;
}
 
bool firstComeFirstServe(vector<process> &process_list, vector<char> &time_chart)
{
   int process_index = 0;
   bool process_started = false;
   int current_cpu_consecutive_idle_time = 0;
   int max_cpu_consecutive_idle_time = 0;
   float quantas = 0.0;
   for(int quanta=0; true; ++quanta)
   {
       quantas++;
       if(process_started == false)
       {
           if(quanta >= 100) break;
           if(process_index<process_list.size() && process_list[process_index].arrival_time <= quanta)
           {
               process_started = true;
               //updating performance metrics
               process_list[process_index].metrics.response_time = quanta - process_list[process_index].arrival_time;
           }
       }
 
       if(process_started)
       {
           time_chart.push_back(process_list[process_index].process_name);
           process_list[process_index].remaining_service_time  = max((float)0, process_list[process_index].remaining_service_time - 1);
          
           if(process_list[process_index].remaining_service_time == 0)
           {
               // updating performance metrics
               process_list[process_index].metrics.turnaround_time = quanta+1 - process_list[process_index].arrival_time;
               process_list[process_index].metrics.waiting_time = process_list[process_index].metrics.turnaround_time - process_list[process_index].service_time;
               ++process_index;
               process_started = false;
           }
           current_cpu_consecutive_idle_time = 0;
       }
       else
       {
           time_chart.push_back('-');
           ++current_cpu_consecutive_idle_time;
           max_cpu_consecutive_idle_time = max(max_cpu_consecutive_idle_time, current_cpu_consecutive_idle_time);
       }
   }
   process_list[ 0 ].metrics.total_quanta = quantas;
   return max_cpu_consecutive_idle_time < 2 ? true : false;
}
 
 //returns the index of a process that has arrived before the current quanta and is the shortest job
int selectProcessForSjf( vector<process> &process_list, int quanta )
{
	//want to select the job that has the shortest runtime
	int min_index = -1;
	float min_service_time = FLT_MAX;
	for ( int i = 0; i < process_list.size(); i++ )
	{
		//if the process has arrived before the current quanta, keep track of index
		if ( process_list[ i ].arrival_time <= quanta )
		{
			if ( process_list[ i ].service_time < min_service_time && process_list[ i ].metrics.turnaround_time == 0 )
			{
				min_service_time = process_list[ i ].service_time;
				min_index = i;
			}
		}
	}
	return min_index;
}

//shortest job first process scheduling algorithm written by Alex Le
bool shortestJobFirst( vector<process> &process_list, vector<char> &time_chart )
{
	int process_index = -1;
	float remaining_runtime = 0.0;
	float total_runtime = 0.0;
	int cpu_idle_time = 0;
	//used for keeping track of total quanta taken for throughput calculation
	float quantas = 0.0;
	//loop until no more run time if there are still processes that arrived before the 100th quanta
	for ( int quanta = 0; quanta < 100 || remaining_runtime > 0; quanta++ )
	{
		quantas++;
		// we look for the shortest job and add it if there are no running jobs
		if ( ( selectProcessForSjf( process_list, quanta ) != -1 ) && ( remaining_runtime <= 0 ) )
		{	
			process_index = selectProcessForSjf( process_list, quanta );
			remaining_runtime = process_list[ process_index ].service_time;
			total_runtime += remaining_runtime;
			process_list[ process_index ].metrics.response_time = quanta - 
				process_list[ process_index ].arrival_time;
			//reset cpu_idle_time since we are adding a new process
			cpu_idle_time = 0;
			process_list[ process_index ].metrics.start_time = quanta;
		}

		//check if currently running a process
		if ( remaining_runtime > 0 )
		{
			remaining_runtime--;
			time_chart.push_back( process_list[ process_index ].process_name );
			// if the runtime goes below zero this quanta, we'll mark it as finished
			// also update performance metrics
			if ( remaining_runtime <= 0 )
			{
				process_list[ process_index ].metrics.turnaround_time = ( quanta + 1 ) -
					process_list[ process_index ].arrival_time;
				process_list[ process_index ].metrics.waiting_time = 			
					process_list[ process_index ].metrics.turnaround_time -
					process_list[ process_index ].service_time;
				process_list[ process_index ].metrics.end_time = quanta +  1;
			}
		}
		else
		{
			cpu_idle_time++;
			if ( cpu_idle_time > 2 )
			{
				//end execution if there is more than 2 consecutive cpu idle quanta
				cout << "CPU IDLE" << endl;
				return false;
			}
		}
	}
	process_list[ 0 ].metrics.total_quanta = quantas;
	return true;
}

int main()
{
  cout << "Choose One of the Process Scheduling Algorithm:" << endl;
  cout << "1. First-come first-served (FCFS) [non-preemptive]" << endl;
  cout << "2. Shortest job first (SJF) [non-preemptive]" << endl;
  cout << "3. Shortest remaining time (SRT) [preemptive]" << endl;
  cout << "4. Round robin (RR) [preemptive]" << endl;
  cout << "5. Highest priority first (HPF) [preemptive]" << endl;
  cout << "6. Highest priority first (HPF) [non-preemptive]" << endl;
 
 
  int algorithm_selected;
  cin >> algorithm_selected;
 
 
  vector<process> process_list;
  vector<vector<process> > workloads;
  int unsuccessful_count = 0;
  int successful_count = 0;
  bool successful = false;
 
 
  vector<char> time_chart;
 
 
  switch(algorithm_selected)
  {
       case 1: while(successful_count<5)
               {
                   createProcessList(process_list, unsuccessful_count*2);
                   time_chart.clear();
                   successful = firstComeFirstServe(process_list, time_chart);                  
                  
                   if(!successful)
                   {
                       cout << "Simulation failed because CPU is idle for more than 2 consecutive quanta." << endl;
                       printTimeChart(time_chart);
                       ++unsuccessful_count;
                   }
                   else
                   {
                       ++successful_count;
                       cout << "Simulation successful." << endl;
                       printProcessList(process_list);
                       printTimeChart(time_chart);
                       workloads.push_back(process_list);
                   }
               }
               calculateAndPrintPerformanceMetrics(workloads);
               break;
        case 2: while(successful_count<5)
               {
                   createProcessList(process_list, unsuccessful_count*2);
                   time_chart.clear();
                   successful = shortestJobFirst(process_list, time_chart);                  
                  
                   if(!successful)
                   {
                       cout << "Simulation failed because CPU is idle for more than 2 consecutive quanta." << endl;
                       printTimeChart(time_chart);
                       ++unsuccessful_count;
                   }
                   else
                   {
                       ++successful_count;
                       cout << "Simulation successful." << endl;
                       printProcessList(process_list);
                       printTimeChart(time_chart);
                       cout << "Processes Executed: " << endl;
                       postPrintProcessList(process_list);
                       workloads.push_back(process_list);
                   }
               }
               calculateAndPrintPerformanceMetrics(workloads);
               break;
      case 4: while(successful_count<5)
              {
                  createProcessList(process_list, unsuccessful_count*2);
                  time_chart.clear();
                  successful = runRoundRobin(process_list, time_chart, workloads);                 
                
                  if(!successful)
                  {
                      cout << "Simulation failed because CPU is idle for more than 2 consecutive quanta." << endl;
                      printTimeChart(time_chart);
                      ++unsuccessful_count;
                  }
                  else
                  {
                      ++successful_count;
                      cout << "Simulation successful." << endl;
                      printProcessList(process_list);
                      printTimeChart(time_chart);
                      // workloads.push_back(process_list);
                  }
              }
              calculateAndPrintPerformanceMetrics(workloads);
              break;
      case 5: while(successful_count<5)
              {
                  createProcessList(process_list, unsuccessful_count*2);
                  time_chart.clear();
                  successful = highestPriorityFirst(process_list, time_chart, true, workloads);                 
                  if(!successful)
                  {
                      cout << "Simulation failed because CPU is idle for more than 2 consecutive quanta." << endl;
                      printTimeChart(time_chart);
                      ++unsuccessful_count;
                  }
                  else
                  {
                      ++successful_count;
                      cout << "Simulation successful." << endl;
                      printProcessList(process_list);
                      printTimeChart(time_chart);
                    //   workloads.push_back(process_list);
                  }
              }
              calculateAndPrintPerformanceMetrics(workloads);
              break;
       case 6: while(successful_count<5)
              {
                  createProcessList(process_list, unsuccessful_count*2);
                  time_chart.clear();
                  successful = highestPriorityFirst(process_list, time_chart, false, workloads);                 
                  if(!successful)
                  {
                      cout << "Simulation failed because CPU is idle for more than 2 consecutive quanta." << endl;
                      printTimeChart(time_chart);
                   ++unsuccessful_count;
                  }
                  else
                  {
                      ++successful_count;
                      cout << "Simulation successful." << endl;
                      printProcessList(process_list);
                     printTimeChart(time_chart);
                    //   workloads.push_back(process_list);
                  }
              }
              calculateAndPrintPerformanceMetrics(workloads);
              break;
      default : cout << "Invalid Input" << endl;
  } 
  return 0;
}


