/* Fill in your Name and GNumber in the following two comment fields
 * Name:Gunim Elkhebri
 * GNumber: G01218571
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"
#include "scheduler.h"
#define EXITCODE_MASK 0xffffff80
Process *insertInOrder(Queue *queue, Process *p);
/*Initialize the Schedule Struct
 * Follow the specification for this function.
 * Returns a pointer to the new Schedule or NULL on any error.
 */
Schedule *scheduler_init() {
	Schedule *schedule =malloc(sizeof(Schedule));
	printf("scheduker_init");						
	if(schedule!=NULL)
	{							
		schedule->ready_queue=malloc(sizeof(Queue));//
		schedule->ready_queue->head=NULL;//initializing head pointer
		schedule->ready_queue->count=0;//initializing field count
			
		schedule->stopped_queue=malloc(sizeof(Queue));//allocationg memory for the linked list
		schedule->stopped_queue->head=NULL;//initializing headpointer
		schedule->stopped_queue->count=0;//initializing count
		
		schedule->waiting_queue=malloc(sizeof(Queue));//allocating memory for linked list
		schedule->waiting_queue->head=NULL;//
		schedule->waiting_queue->count=0;//initializing count
		return schedule;		// return our scheduler
	}
  return NULL;
}

/* Add the process into the appropriate linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_add(Schedule *schedule, Process *process) {
	printf("scheduler_add\n");       	
	
	if(process!=NULL){
	int r=process->flags&0x7F;									
	if((r&RUNNING)!=0||(r&READY)!=0||(r&CREATED)!=0){
							
	process->flags|=READY;//created bit still on
	process->flags&=0xffffff80+READY;
												
	schedule->ready_queue->head=insertInOrder(schedule->ready_queue,process);		
	

		return 0;	
	}	


  	
	else  
					

  return -1;

}
return -1;
}


				
Process *insertInOrder(Queue *ready,Process *p){
	  printf("inOrder_add\n");  								
	
					
	if(ready->head==NULL){
	
	ready->head=p;
	return ready->head;
		
	}				
				
	Process *curr = ready->head;
	Process *prev=NULL;
								
	while(curr!=NULL&&curr->pid < p->pid){
							
	prev=curr;
	curr=curr->next;
	
	}
	
	
	if(prev!=NULL){
		
	prev->next=p;			
	}
	
	else {

	ready->head=p;
	
		

	}

	p->next=curr;
	
	


return ready->head;
		
}




/* Receives the process from the CPU and puts it into the Waiting Queue
 * Follow the specification for this function.
 * Returns a 0 on success or -1 on any error.
 */
int scheduler_wait(Schedule *schedule, Process *process, int io_time) {
	  printf("scheduler_wait\n");  
	if(schedule==NULL||process==NULL){
		
	return -1;//failure
		
	}
		
	process->flags|=WAITING;//created bit still on
        process->flags&=(0xffffff80+WAITING);//now only ready bit is on
	process->wait_remaining=io_time;


	Process *curr = schedule->waiting_queue->head;
	
	
	
	if(curr==NULL){
				
	schedule->waiting_queue->head=process;//insert at head due to value null
	}
				
	else if(curr->next==NULL){
				
	curr->next=process;//insert at second due to 2nd value NULL
	
	}
	
	else {
			
		while(curr->next!=NULL){
							
		curr=curr->next;//traversing list
	
		}
				
	
		curr->next=process;//inserting process at the end of list

	}
	

	


  return 0;
}


/* Receives a process from the CPU and moves it to the stopped queue.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_stop(Schedule *schedule, Process *process) {
  
	if(process==NULL||schedule==NULL) {
	
	return -1;//error
	
	} 
	 printf("scheduler_stop\n");  
			
	Queue *stop=schedule->stopped_queue;
													
				
	process->flags|=STOPPED;
        process->flags&=(0xffffff80+STOPPED);

						
	stop->head=insertInOrder(schedule->stopped_queue,process);	
//	Queue *curr=stop;

//	Queue *prev = NULL;



//	while(curr!=NULL&&curr->pid<process->pid){


//	prev=curr;

//	curr=curr->next;
	
//	}


//	if(prev!=NULL){

//	prev->next=process;//insert in order
				
//	}				

//	else

//	stop=process;//lowest pid so it goes in the front


//	process->next=curr;

		
return 0;
}

/* Move the process with matching pid from Stopped to Ready.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_continue(Schedule *schedule, int pid) {
		

	
	
	Process *curr = schedule->stopped_queue->head; 							
	Process *prev =NULL;
						
			
	int r = 0;
		while(curr!=NULL)
		{
							
			if(curr->pid==pid){
			r=1;
			break;		
			}			
			
			prev=curr;

			curr=curr->next;		
						
		}

	
		if(r==0){

		return -1;//not found
				
		}


	if(prev!=NULL){
						
		prev->next=curr->next;//remove curr so we skip over curr to point to curr->next
					
	}
	else{
								
		schedule->stopped_queue->head=curr->next;//prev is null so remove head
				
	}

						
	Queue *queue=schedule->stopped_queue;

	queue->count-=1;
	curr->flags|=READY;//story ready bit and exit codes
        curr->flags&=(0xffffff80+READY);//now only ready bit is on	
	schedule->ready_queue->head=insertInOrder(schedule->ready_queue,curr);
								

		
 return 0;
}

/* Receives the process from the CPU for removal.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int scheduler_finish(Process *process) {
	 printf("scheduler_finish\n");
	int exitcode=0;
	if(process->time_remaining==0) {
				
	    
				
	exitcode=((unsigned)process->flags)>>FLAG_BITS;   
		
	
	
	free(process->command);
	free(process);
	
	return exitcode;	
	
	//extract exit code	

	}
							



  return -1;
}

/* Create a new Process with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Follow the specification for this function.
 * Returns the Process on success or a NULL on any error.
 */
Process *scheduler_generate(char *command, int pid, int time_remaining, int is_sudo) {
	 printf("scheduler_generate\n");
	Process *process=malloc(sizeof(Process));//allocating memory for struct
	char *clone=malloc(sizeof(char)*(strlen(command)+1));//cloning the string since its a pointer and 1 for null terminator
	strncpy(clone,command,sizeof(command+1));
																									
	process->command=clone;
	
	process->pid=pid;		
	process->time_remaining=time_remaining;
	process->time_last_run= clock_get_time();


	

	process->wait_remaining=0;

	process->flags=0x0|CREATED;//clearing exits bits first and turning on created bit
	
	if(is_sudo==1){
	
	process->flags|=0x1;//turning on sudo bit
														
	}	

	return process;// returning process
}									

/* Select the next process to run from Ready Queue.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 */
Process *scheduler_select(Schedule *schedule) {
	Queue *queue=schedule->ready_queue;
	//Process *head=schedule->readyqueue->head
	//not setting to head becasue we want to modidy the head 					
	int min=TIME_STARVATION;				
	Process* curr;
	Process *prev;
	Process *selectedPrev;
	curr=queue->head;
	prev=NULL;
	selectedPrev=NULL;
	

	while(curr!=NULL){
										
	if(clock_get_time()-curr->time_last_run>=TIME_STARVATION){
	//checking for starvation
	selectedPrev=prev;
	break;
	
	}
	if(curr->time_remaining<min){
					
	min=curr->time_remaining;//finding the min time remaining
					
	selectedPrev=prev;
	
	}
	
	prev=curr;
	curr=curr->next;
			
	}

		
	
	//Process *selected=selectedPrev->next;

	Process *selected;

	
	if(selectedPrev==NULL){
								
	selected=queue->head;//
								
	}
																
	else{
						
	selected=selectedPrev->next;//
	} //from line 405-414 determing whats selected is = to

	if(selectedPrev==NULL){
									
	queue->head=selected->next;//removing selected element from queue from line 416 to end of else to line 426
	}														
																												
	else{

	

	selectedPrev->next=selected->next;//skipping over selected
	}				
	selected->next=NULL;//detatching it from list	
			
	selected->flags&=(-1<<FLAG_BITS)|RUNNING;
								
 return selected;
}

/* Updates the first process in the waiting queue by decrementing waiting_time.
 * If that process is done (waiting_time is 0), move it to the ready queue.
 * Follow the specification for this function.
 * Returns 0 on success or -1 on any errors.
 */
int scheduler_io_run(Schedule *schedule) {
 	 printf("scheduler_iorun\n");	
	if(schedule==NULL||schedule->waiting_queue==NULL){
	return -1;

	}
	
	Queue *queue=schedule->waiting_queue;
					
	Process *process=queue->head;						
	
						
	if(process!=NULL&&--process->wait_remaining==0){
	
	queue->head=process->next;

	

	 process->flags|=READY;
        process->flags&=(0xffffff80+READY);	
					
	schedule->ready_queue->head=insertInOrder(schedule->ready_queue,process);	
	}
	

 return 0;
}

/* Returns the number of items in a given Linked List (Queue) (Queue)
 * Follow the specification for this function.
 * Returns the count of the Linked List, or -1 on any errors.
 */
int scheduler_count(Queue *ll) {
 	 printf("scheduler_count\n");
	
  int count=0;
	if(ll==NULL){
	 return -1;
	}
				
	Process *pro;
	pro=ll->head;//setting pro at head to traverse the list to count nodes
	
	while(pro!=NULL){
	count++;
	
	pro=pro->next;

	}

	return count;
	
}

/* Completely frees all allocated memory in the scheduler
 * Follow the specification for this function.
 */			
void freeQueue(Queue *queue){
	
	if(queue==NULL){
	return;
	}
	 printf("freeQueue\n");
	Process *curr=queue->head;//temp var to keep track of next node 
	Process *temp=NULL;
	
	while(curr!=NULL){
						
		temp=curr->next;//temp var storing are next node
		free(curr->command);//free pointer command which was also allocated
		free(curr);//free struct
		curr=temp;//assigning to next node		
	}

	free(queue);

return;
}

void scheduler_free(Schedule *scheduler) {
	 printf("scheduler_free\n");				
	freeQueue(scheduler->ready_queue);
        freeQueue(scheduler->stopped_queue);
	freeQueue(scheduler->waiting_queue);



			
  return;
}
