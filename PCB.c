#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define BUFFSIZE 10


typedef struct PCB_s *PCB;
typedef struct PCB_s{
	PCB child;
	int processID; //pid
	//char pname[BUFFSIZE]; //processname
	int priority;
	int state;
}PCB_t[1];

typedef struct LINKED_QUEUE_NODE_s *LINKED_QUEUE_NODE;
typedef struct LINKED_QUEUE_NODE_s{
	PCB data;
	LINKED_QUEUE_NODE next;
}LINKED_QUEUE_NODE_t[1];


typedef struct LINKED_QUEUE_s *LINKED_QUEUE;
typedef struct LINKED_QUEUE_s{
	LINKED_QUEUE_NODE head;
	LINKED_QUEUE_NODE tail;
}LINKED_QUEUE_t[1];


PCB init_PCB(int pid, int priority, int state){
	PCB pcb = (PCB)malloc(sizeof(PCB_t));
	pcb->processID = pid;
	pcb->priority = priority;
	pcb->state = 0;
	pcb->child = NULL;
	return pcb;
}

LINKED_QUEUE linked_queue_init(){
	LINKED_QUEUE queue = (LINKED_QUEUE)malloc(sizeof(LINKED_QUEUE_t) * BUFFSIZE);
	if(queue == NULL){
		printf("QUEUE not initialized");
	}
	else{
		queue->head = NULL;
		queue->tail = NULL;
	}
	return queue;
}

PCB pt[BUFFSIZE];

void linked_queue_enqueue(int ProcessID, LINKED_QUEUE queue){
	PCB pcb = pt[ProcessID];
	LINKED_QUEUE_NODE node = (LINKED_QUEUE_NODE)malloc(sizeof(LINKED_QUEUE_NODE_t));
	node->data = pcb;
	node->next = NULL;

	if(queue->head == NULL){
		queue->head = node;
		queue->tail = node;
	}
	else if(pcb->priority < queue->head->data->priority){
		node->next = queue->head;
		queue->head = node;
	}
	else{
		LINKED_QUEUE_NODE pos = queue->head;
		while(pos->next != NULL && pcb->priority >= pos->next->data->priority){
			pos = pos->next;
		}
		node->next = pos->next;
		pos->next = node;
		if(node->next == NULL){
			queue->tail = node;
		}
	}
}



void *dequeue(LINKED_QUEUE queue){
	LINKED_QUEUE_NODE node;
	void *result = NULL;
	node = queue->head;
	if(node != NULL){
		if(node == queue->tail){
			queue->head = queue->tail = NULL;
		}
		else{
			queue->head = node->next;
		}
		result = node->data;
		free(node);
	}
	else{
		printf("Queue Empty!");
	}
	return result;
}

void insert(PCB pcb){
	for(int i = 0; i <= BUFFSIZE; i++){
		pt[pcb->processID] = pcb;
	}
}


void print(LINKED_QUEUE queue){
	LINKED_QUEUE_NODE node = (LINKED_QUEUE_NODE)malloc(sizeof(LINKED_QUEUE_NODE));
	if((queue->head == NULL) && (queue->tail == NULL)){
		printf("Queue is empty");
	}
	else{
		node = queue->head;
		while(node){
			printf("%d>",  node->data->processID);
			node = node->next;
		}
		printf("\n\n");
	}
}

void removePT(int processID){
	pt[processID] = NULL;
}

void remove_queue(int processID, LINKED_QUEUE queue){
	if(queue == NULL){
		printf("Queue empty");
	}
	else{
		LINKED_QUEUE_NODE currnode = queue->head;
		LINKED_QUEUE_NODE prevnode = NULL;
		if(currnode != NULL){
			if(currnode->data->processID == processID){
				if(prevnode == NULL){
					queue->head = currnode->next;
				}
			}
		}
		while(currnode != NULL &&currnode->data->processID != processID){
			prevnode = currnode;
			currnode = currnode->next;
		}
		if(currnode == NULL){
			return;
		}
		if(currnode == queue->tail){
			queue->tail = prevnode;
			prevnode->next = NULL;
			return;
		}
		prevnode->next = currnode->next;
	}
}

bool emptypt(){
	for(int i = 0; i <= BUFFSIZE;i++){
		if(pt[i] == NULL){
			return true;
		}
	}
	return false;
}

int main(){
	for (int i = 0; i < 10; i++) {
	        pt[i] = init_PCB(i, i, 0);
	    }
	    LINKED_QUEUE queue = linked_queue_init();

	    linked_queue_enqueue(3, queue);
	    linked_queue_enqueue(7, queue);
	    linked_queue_enqueue(2, queue);
	    printf("Queue after enqueue:\n");
	    print(queue);

	    printf("Dequeueing a process from the queue\n");
	    PCB pcb = dequeue(queue);
	    printf("Dequeued process: %d\n", pcb->processID);
	    printf("Queue after dequeue:\n");
	    print(queue);


	    printf("Enqueueing a process with PID 1 to the queue:\n");
	    linked_queue_enqueue(1, queue);
	    printf("Queue after enqueue:\n");
	    print(queue);

	    printf("Removing a process with PID 7 from the queue:\n");
	    remove_queue(7, queue);
	    removePT(7);
	    printf("Queue after removal:\n");
	    print(queue);


	    LINKED_QUEUE empty_queue = linked_queue_init();


	    insert(init_PCB(3, 3, 0));
	    insert(init_PCB(7, 7, 0));
	    insert(init_PCB(2, 2, 0));
	    insert(init_PCB(5, 5, 0));
	    for (int i = 0; i < BUFFSIZE; i++) {
	        if (pt[i] != NULL) {
	            linked_queue_enqueue(i, empty_queue);
	        }
	    }
	    printf("Empty queue after insertion:\n");
	    print(empty_queue);

	    return 0;
}
