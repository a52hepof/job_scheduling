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
};


void read(struct process *prcs){
	printf("Enter Process Name: "); scanf("%s", prcs->name);
	printf("Enter Arrival Time: "); scanf("%d", &prcs->arrival_time);
	printf("Enter Burst Time  : "); scanf("%d", &prcs->burst_time);
	prcs->completion_time = prcs->turn_around_time = prcs->waiting_time = prcs->response_time = 0;
	prcs->remaining_time = prcs->burst_time;
	printf("\n");
}

void printdata(int num_of_process, struct process prscs[num_of_process]){
	float avg_waiting_time = 0;
	printf(" _______________________________________________________________________________________________________________ \n");
	printf("|  Process Name  | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time | Response Time |\n");
	printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");
	for(int i=0;i<num_of_process;i++){
		printf("|%-16s|%-14d|%-12d|%-17d|%-17d|%-14d|%-15d|\n", prscs[i].name, prscs[i].arrival_time, prscs[i].burst_time, prscs[i].completion_time, prscs[i].turn_around_time, prscs[i].waiting_time, prscs[i].response_time);
		avg_waiting_time += prscs[i].waiting_time;
	}
	printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");
	avg_waiting_time /= num_of_process;
	printf("Average waiting time: %.2f\n", avg_waiting_time);
}


void copy(struct process *to, struct  process from){
	strcpy(to->name, from.name);
	to->arrival_time = from.arrival_time;
	to->burst_time = from.burst_time;
	to->completion_time = from.completion_time;
	to->turn_around_time = from.turn_around_time;
	to->waiting_time = from.waiting_time;
	to->response_time = from.response_time;
}

void FCFS(int num_of_process, struct process prscs[num_of_process]){
	
	for(int i=0;i<num_of_process-1;i++){
		struct process curr;
		copy(&curr, prscs[i]);
		int min = i;
		int j=i+1;
		for(j=i+1;j<num_of_process;j++){
			if(prscs[j].arrival_time < curr.arrival_time){
				copy(&curr, prscs[j]);
				min = j;
			}
		}
		if(min!=i){
			copy(&prscs[min], prscs[i]);
			copy(&prscs[i], curr);
		}
	}

	prscs[0].completion_time = prscs[0].burst_time + prscs[0].arrival_time;
	prscs[0].waiting_time = 0;
	prscs[0].turn_around_time = prscs[0].completion_time - prscs[0].arrival_time;

	for(int i=1;i<num_of_process;i++){
		prscs[i].completion_time = prscs[i-1].completion_time + prscs[i].burst_time;
		prscs[i].turn_around_time = prscs[i].completion_time - prscs[i].arrival_time;
		prscs[i].waiting_time = prscs[i].turn_around_time - prscs[i].burst_time;
		prscs[i].response_time = prscs[i].waiting_time + prscs[i].burst_time;
	}
}

int shortestJob(int num_of_process, struct process prscs[num_of_process], int elapsed_time, int done[num_of_process]){
	struct process shortest;
	shortest.burst_time = INT_MAX;
	int shortest_index = -1;
	for(int i=0; i<num_of_process; i++){
		if(prscs[i].arrival_time <= elapsed_time && prscs[i].burst_time < shortest.burst_time && !done[i]){
			 shortest_index = i;
			 copy(&shortest, prscs[i]);
		}
	}
	return shortest_index;
}

void SJF(int num_of_process, struct process prscs[num_of_process]){
	for(int i=0;i<num_of_process-1;i++){
		struct process curr;
		copy(&curr, prscs[i]);
		int min = i;
		int j=i+1;
		for(j=i+1;j<num_of_process;j++){
			if(prscs[j].arrival_time < curr.arrival_time){
				copy(&curr, prscs[j]);
				min = j;
			}
			else if(prscs[j].arrival_time == curr.arrival_time && prscs[j].burst_time < curr.burst_time){
				copy(&curr, prscs[j]);
				min = j;
			}
		}
		if(min!=i){
			copy(&prscs[min], prscs[i]);
			copy(&prscs[i], curr);
		}   
	} 
	int time_elapsed = prscs[0].arrival_time;
	int done[num_of_process];
	for(int i=0; i<num_of_process; i++) done[i] = 0;
	
	int job = shortestJob(num_of_process, prscs, time_elapsed, done);
	while(job!=-1){
		prscs[job].completion_time = time_elapsed + prscs[job].burst_time;
		prscs[job].turn_around_time = prscs[job].completion_time - prscs[job].arrival_time;
		time_elapsed += prscs[job].burst_time;
		prscs[job].waiting_time = prscs[job].turn_around_time - prscs[job].burst_time;
		prscs[job].response_time = prscs[job].waiting_time + prscs[job].burst_time;
		done[job] = 1;
		job = shortestJob(num_of_process, prscs, time_elapsed, done);
	}
	

}


int main(){
	int num_of_process;
	printf("Enter number of process: ");
	scanf("%d" , &num_of_process);
 	struct process processes[num_of_process];
	for(int i=0;i < num_of_process; i++) read(&processes[i]);
	printf("First come first serve output: \n ");
	FCFS(num_of_process, processes);
	printdata(num_of_process, processes);
	printf("\nShortest Job First output: \n");
	SJF(num_of_process, processes);
	printdata(num_of_process, processes);

}
