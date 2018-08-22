#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
struct process{
	char name[15];
	int arrival_time;
	int burst_time;
	int completion_time;
	int turn_around_time;
	int waiting_time;
	int response_time;
	int remaining_time;
	int priority;
};

int idle_time;
int total_burst;
void read(struct process *prcs){
	printf("Enter Process Name: "); scanf("%s", prcs->name);
	printf("Enter Arrival Time: "); scanf("%d", &prcs->arrival_time);
	printf("Enter Burst Time  : "); scanf("%d", &prcs->burst_time);
	printf("Enter Priority    : "); scanf("%d", &prcs->priority);
	prcs->completion_time = prcs->turn_around_time = prcs->waiting_time = prcs->response_time = 0;
	prcs->remaining_time = prcs->burst_time;
	total_burst += prcs->burst_time;
	printf("\n");
}

void printdata(int num_of_process, struct process prscs[num_of_process]){
	float avg_waiting_time = 0;
	float utilization = 0;
	printf(" _______________________________________________________________________________________________________________ \n");
	printf("|  Process Name  | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time | Response Time |\n");
	printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");
	for(int i=0;i<num_of_process;i++){
		printf("|%-16s|%-14d|%-12d|%-17d|%-17d|%-14d|%-15d|\n", prscs[i].name, prscs[i].arrival_time, prscs[i].burst_time, prscs[i].completion_time, prscs[i].turn_around_time, prscs[i].waiting_time, prscs[i].response_time);
		avg_waiting_time += prscs[i].waiting_time;
	}
	printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");
	avg_waiting_time /= num_of_process;
	printf("Average waiting time : %.2f\n", avg_waiting_time);
	utilization = (total_burst / (idle_time + total_burst)) * 100;
	printf("CPU utilization      : %.2f%%\n", utilization);
}

void resetRemainingTime(int num_of_process, struct process prscs[num_of_process]){
	for(int i=0;i<num_of_process; i++){
		prscs[i].remaining_time = prscs[i].burst_time;
	}
}
void copy(struct process *to, struct  process from){
	strcpy(to->name, from.name);
	to->arrival_time = from.arrival_time;
	to->burst_time = from.burst_time;
	to->completion_time = from.completion_time;
	to->turn_around_time = from.turn_around_time;
	to->waiting_time = from.waiting_time;
	to->response_time = from.response_time;
	to->remaining_time = from.remaining_time;
}

int allDone(int num_of_process, int done[num_of_process]){
    for(int i=0;i<num_of_process;i++){
        if(done[i]==0) return 0;
    }   
    return 1;
}


int firstJob(int num_of_process, struct process prscs[num_of_process], int time_elapsed, int done[num_of_process]){
	int jobIndex = -1;
	int earliest = INT_MAX;
	for(int i=0;i < num_of_process; i++){
		if(!done[i] && prscs[i].arrival_time <= earliest){
			jobIndex = i;
			earliest = prscs[i].arrival_time;
		}
	}
	return jobIndex;
}

void FCFS(int num_of_process, struct process prscs[num_of_process]){
	idle_time = 0;
	int done[num_of_process];
	for(int i=0;i<num_of_process;i++) done[i] = 0;
	int time_elapsed = 0;
	
	int i; 
	while(!allDone(num_of_process, done)){
		i = firstJob(num_of_process, prscs, time_elapsed, done);
		if(i == -1) {
			time_elapsed++;
			idle_time++;
			continue;
		}
		time_elapsed += prscs[i].burst_time;
		prscs[i].completion_time =  time_elapsed;
		prscs[i].turn_around_time = prscs[i].completion_time - prscs[i].arrival_time;
		prscs[i].waiting_time = prscs[i].turn_around_time - prscs[i].burst_time;
		prscs[i].response_time = prscs[i].waiting_time;
		done[i] = 1;
	}
}

int shortestJob(int num_of_process, struct process prscs[num_of_process], int elapsed_time, int done[num_of_process]){
	
	int shortest_burst = INT_MAX;
	int shortest_index = -1;
	for(int i=0; i<num_of_process; i++){
		if(prscs[i].arrival_time <= elapsed_time && prscs[i].burst_time < shortest_burst && !done[i]){
			 shortest_index = i;
			 shortest_burst = prscs[i].burst_time;
		}
	}
	return shortest_index;
}

void SJF(int num_of_process, struct process prscs[num_of_process]){

	idle_time = 0;
	int time_elapsed = 0;
	int done[num_of_process];
	for(int i=0; i<num_of_process; i++) done[i] = 0;
	
	int job;
	while(!allDone(num_of_process, done)){
		job = shortestJob(num_of_process, prscs, time_elapsed, done);
		if(job == -1){
			time_elapsed++;
			idle_time++;
			continue;
		}
		prscs[job].completion_time = time_elapsed + prscs[job].burst_time;
		prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
		time_elapsed += prscs[job].burst_time;
		prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
		prscs[job].response_time = prscs[job].waiting_time;
		done[job] = 1;
		
	}
}

void roundRobin(int num_of_process, struct process prscs[num_of_process], int time_quantum){
	idle_time = 0;
	int done[num_of_process];
	for(int i=0;i<num_of_process;i++) done[i] = 0;
	int time_elapsed = 0;
	int lt = time_elapsed;
	while(!allDone(num_of_process, done)){
		for(int i=0;i<num_of_process;i++){
			//see if the process has arrived and incomplete
			if(prscs[i].arrival_time <= time_elapsed && !done[i]){ 
				//the process has arrived let's check if has any thing remaining 
				if(prscs[i].remaining_time > 0){
					//there's something to execute 
					
					//if it's executing for the first time, calculate the response time.
					if(prscs[i].remaining_time == prscs[i].burst_time){
						prscs[i].response_time = time_elapsed - prscs[i].arrival_time;
					}
					//let's see if it requries less than or greater than equal to our quantum time
					if(prscs[i].remaining_time <= time_quantum){
						//process
						//the prscs[i] will comeplete in this cycle let's calulate it's details and set it done
						time_elapsed += prscs[i].remaining_time;
						prscs[i].completion_time = time_elapsed;
						prscs[i].remaining_time = 0;
						done[i] = 1;
						prscs[i].turn_around_time = prscs[i].completion_time - prscs[i].arrival_time;
						prscs[i].waiting_time = prscs[i].turn_around_time - prscs[i].burst_time;
					}
					else{
						//there's something to execute, but the process wont be done here
						//we step one more quantum forward in time
						//and process that much of the current process
						time_elapsed += time_quantum;
						prscs[i].remaining_time -= time_quantum;
					}
				}
			}
		}
		if(lt == time_elapsed){
			idle_time++;
			time_elapsed++;
		}
		lt= time_elapsed;
	}
}

int mostPriorJob(int num_of_process, struct process prscs[num_of_process], int time_elapsed, int done[num_of_process]){
	int priorIndex = -1;
	int most_prior = INT_MAX;

	for(int i=0; i < num_of_process; i++){
		if(!done[i] && prscs[i].arrival_time <= time_elapsed && prscs[i].priority < most_prior){
			priorIndex = i;
			most_prior = prscs[i].priority;
		}
	}

	return priorIndex;
}

void nonPreemptive_Priority(int num_of_process, struct process prscs[num_of_process]){
	idle_time = 0;
	int done[num_of_process];
	for(int i=0; i<num_of_process;i++) done[i] = 0;
	int time_elapsed = 0;
	int job = -1;
	
	while(!allDone(num_of_process, done)){
		job = mostPriorJob(num_of_process, prscs, time_elapsed, done);
		if(job==-1){
			idle_time++;
			time_elapsed++;
			continue;
		}
		time_elapsed += prscs[job].burst_time;
		prscs[job].completion_time = time_elapsed;
		prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
		prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
		done[job] = 1;
		prscs[job].response_time = prscs[job].waiting_time;
	}
}

void Preemptive_Priority(int num_of_process, struct process prscs[num_of_process]){
	idle_time = 0;
	int done[num_of_process];
	for(int i=0; i< num_of_process; i++) done[i] = 0;
	int time_elapsed = 0;
	int job = -1;
	for(; !allDone(num_of_process, done);){
		job = mostPriorJob(num_of_process, prscs, time_elapsed, done);
		if(job == -1){
			idle_time++;
			time_elapsed++;
			continue;
		}
		time_elapsed++;
		if(prscs[job].remaining_time == prscs[job].burst_time){
			prscs[job].response_time = time_elapsed - prscs[job].arrival_time;
		}
		prscs[job].remaining_time -= 1;
		if(prscs[job].remaining_time == 0){ //the job completed
			prscs[job].completion_time = time_elapsed;
			prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
			prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
			done[job] = 1;
		}
	}
}

int SRTJob(int num_of_process, struct process prscs[num_of_process], int time_elapsed, int done[num_of_process]){
	int jobIndex = -1;
	int shortestTime = INT_MAX;
	for(int i=0; i<num_of_process; i++){
		if(!done[i] && prscs[i].arrival_time <= time_elapsed && prscs[i].remaining_time < shortestTime){
			shortestTime = prscs[i].remaining_time;
			jobIndex = i;
		}
	}
	return jobIndex;
}
void SRTF(int num_of_process, struct process prscs[num_of_process]){
	idle_time = 0;
	int done[num_of_process];
	for(int i=0;i < num_of_process; i++) done[i] = 0;
	int time_elapsed = 0;
	int job = -1;
	while(!allDone(num_of_process, done)){
		job = SRTJob(num_of_process, prscs, time_elapsed, done);
		if(job == -1){
			time_elapsed++;
			idle_time++;
			continue;
		}
		if(prscs[job].remaining_time == prscs[job].burst_time){
			prscs[job].response_time = time_elapsed - prscs[job].arrival_time;
		}
		time_elapsed++;
		prscs[job].remaining_time -= 1;
		if(prscs[job].remaining_time == 0){
			prscs[job].completion_time = time_elapsed;
			prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
			prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
			done[job] = 1;
		}
	}
}

int main(){
	int num_of_process;
	int time_quantum;

	printf("Enter number of process: ");
	scanf("%d" , &num_of_process);
 	printf("Enter time quantum for round-robin: "); scanf("%d", &time_quantum);
	struct process processes[num_of_process];
	for(int i=0;i < num_of_process; i++) read(&processes[i]);
	
	printf("First come first serve output: \n ");
	FCFS(num_of_process, processes);
	printdata(num_of_process, processes);
	

	printf("\nShortest Job First output: \n");
	SJF(num_of_process, processes);
	printdata(num_of_process, processes);
	
	printf("\nRound robin output: \n");
	roundRobin(num_of_process, processes, time_quantum);
	printdata(num_of_process, processes);
	resetRemainingTime(num_of_process, processes);

	printf("\nNon-Preemptive Priority output: \n");
	nonPreemptive_Priority(num_of_process, processes);
	printdata(num_of_process, processes);
	
	printf("\nPreemptive Priority output: \n");
	Preemptive_Priority(num_of_process, processes);
	printdata(num_of_process, processes);
	resetRemainingTime(num_of_process, processes);
	
	printf("\nShortest Running time output: \n");
	SRTF(num_of_process, processes);
	printdata(num_of_process, processes);
}
