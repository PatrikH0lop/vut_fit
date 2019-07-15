/*
	Author : Patrik Holop, xholop01@stud.fit.vutbr.cz
	Name   : proj2.c
	Date   : April 2017
	Project: Managing processes, IOS
*/

//Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

//Defined Values
#define WRONG_ARG -1
#define ERROR -1

//ARGUMENTS
int A = 0; // adults to create
int C = 0; // children to create
int AGT = 0; // waiting time to next fork (adult)
int CGT = 0; // waiting time to next fork (child)
int AWT = 0; // adult waiting in center (time)
int CWT = 0; // child waiting in center (time)

//SEMAPHORES
sem_t *parentResponse; // parent responding for child
sem_t *childResponse; // child responding for parent
sem_t *mutex; // lock critical sections
sem_t *mutex_multi; // ended processes counter
sem_t *barrier; // hold finished processes
sem_t *childQueue; // children entering
sem_t *adultQueue; // parents leaving

//GLOBAL VARIABLES
FILE *output;
int *steps; // step counter
int *adultsInCenter; // number of adults in center
int *childrenInCenter; // number of childer in center
int *adultsCreated; // created adult processes
int *ended_p; // already ended processes
int *waiting; // number of processes to wait
int *leaving; // number of process to leave


//FUNCTIONS
int process_arguments(int argc, char *argv[]);
int open_semaphores();
int close_semaphores();
int init_variables();
int deinit_variables();
void handle_error(char *message);

int child_process();
int adult_process();

//MAIN
int main(int argc, char *argv[])
{
	//open file and turn of buffering
	output = fopen("proj2.out","wt");
	setbuf(output, NULL);
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    //initialize project
	open_semaphores();
	init_variables();
	
	//time random
	srand(time(NULL));

	//set variables
	*childrenInCenter = 0;
	*adultsInCenter = 0;
	*steps = 1;
	*ended_p = 0;
	*adultsCreated = 0;
    *waiting = 0;
    *leaving = 0;

    //check arguments
    int input_state = process_arguments(argc,argv);
    if (input_state == WRONG_ARG) {
        fprintf(stderr,"Wrong arguments\n");
        close_semaphores();
        deinit_variables();
        return 1;
    }

    //PROCESSES OF ADULTS AND CHILDREN
    //I(identifier) = position in array+1 
    pid_t adults[A];
    pid_t children[C];
    for (int i = 0; i<A; i++)
    	adults[i]=-1;
    for (int i = 0; i<C; i++)
    	children[i]=-1;

    //GENERATORS
    pid_t child_gen;
    pid_t adult_gen = fork();

    if (adult_gen == 0) {   	
    	for (int i = 0; i<A; i++) {
    		usleep(1000*(rand()%(AGT+1)));
    		//forking
    		adults[i]=fork();
    		if (adults[i]==0) {
    			adult_process(i);			
    		}	
    		else if (adults[i]<0) {
    			handle_error("Parent forking error");
    		}
    	}
    	exit(0);
    }
    else if (adult_gen > 0) {
    	child_gen = fork();
    	if (child_gen == 0) {
    		for (int i = 0; i<C; i++) {
    			usleep(1000*(rand()%(CGT+1)));
    			children[i]=fork();
    			if (children[i]==0) {
                     child_process(i);
                }    			
                else if (children[i]<0) {
                	handle_error("Child forking error");
                }		
    		}
    		exit(0);
   		}
    	else if (child_gen > 0) {
    		//printf("Hlavny program\n");
    	}
    	else {
    		handle_error("Forking error\n");
    	}
    }
    else {
    	handle_error("Forking error\n");
    }
    //WAIT FOR ALL TO FINISH
    if (*ended_p == (A+C)) {
    	sem_post(barrier);
    }
    sem_wait(barrier);
    sem_post(barrier);

    deinit_variables();
    close_semaphores();

	fclose(output);
   	return 0;
}

//FUNCTIONS
int process_arguments(int argc, char *argv[]) {
	if (argc == 7) {
		char *x_test;
		//test A
		A = strtol(argv[1],&x_test,10);
		if (*x_test!='\0' || A<=0)
			return WRONG_ARG;		//test C
		C = strtol(argv[2],&x_test,10);
		if (*x_test!='\0' || C<=0)
			return WRONG_ARG;

		//test AGT
		AGT = strtol(argv[3],&x_test,10);
		if (*x_test!='\0' || AGT<0 || AGT>=5001)
			return WRONG_ARG;
		//test CGT
		CGT = strtol(argv[4],&x_test,10);
		if (*x_test!='\0' || CGT<0 || CGT>=5001)
			return WRONG_ARG;

		//test AWT
		AWT = strtol(argv[5],&x_test,10);
		if (*x_test!='\0' || AWT<0 || AWT>=5001)
			return WRONG_ARG;
		//test CWT
		CWT = strtol(argv[6],&x_test,10);
		if (*x_test!='\0' || CWT<0 || CWT>=5001)
			return WRONG_ARG;
		return 0;
	}
	else {
		return WRONG_ARG;
	}
}

int open_semaphores() {
	if ((parentResponse = sem_open("/parentResponse", O_CREAT | O_EXCL, S_IWUSR | S_IRUSR, 0))==SEM_FAILED)
		handle_error("semaphore init error");
	if ((childResponse = sem_open("/childResponse", O_CREAT | O_EXCL, S_IWUSR | S_IRUSR, 0))==SEM_FAILED)
		handle_error("semaphore init error");
	if ((mutex = sem_open("/mutex", O_CREAT | O_EXCL, S_IWUSR | S_IRUSR, 1))==SEM_FAILED)
		handle_error("semaphore init error");
    if ((mutex_multi = sem_open("/mutex_multi", O_CREAT | O_EXCL, S_IWUSR | S_IRUSR, 1))==SEM_FAILED)
    	handle_error("semaphore init error");
    if ((barrier = sem_open("/barrier", O_CREAT | O_EXCL, S_IWUSR | S_IRUSR, 0))==SEM_FAILED)
    	handle_error("semaphore init error");    
    if ((adultQueue = sem_open("/adultQueue", O_CREAT | O_EXCL, S_IWUSR | S_IRUSR, 0)) == SEM_FAILED)
    	handle_error("semaphore init error");
    if ((childQueue = sem_open("/childQueue", O_CREAT | O_EXCL, S_IWUSR | S_IRUSR, 0))==SEM_FAILED)
    	handle_error("semaphore init error");
    return 0;
}

int close_semaphores() {
	if (sem_unlink("/parentResponse")==-1 || sem_close(parentResponse)==-1) {
		fprintf(stderr, "semaphore close error\n");
		exit(ERROR);
	};
	if (sem_unlink("/childResponse")==-1 || sem_close(childResponse)==-1) {
		fprintf(stderr, "semaphore close error\n");
		exit(ERROR);
	};
	if (sem_unlink("/mutex")==-1 || sem_close(mutex)==-1) {
		fprintf(stderr, "semaphore close error\n");
		exit(ERROR);
	}; 
    if (sem_unlink("/barrier")==1 || sem_close(barrier)==-1){
		fprintf(stderr, "semaphore close error\n");
		exit(ERROR);
	};
    if (sem_unlink("/adultQueue")==-1 || sem_close(adultQueue)==-1){
		fprintf(stderr, "semaphore close error\n");
		exit(ERROR);
	};
    if (sem_unlink("/childQueue")==-1 || sem_close(childQueue)==-1){
		fprintf(stderr, "semaphore close error\n");
		exit(ERROR);
	};
    if (sem_unlink("/mutex_multi")==-1 || sem_close(mutex_multi)==-1){
		fprintf(stderr, "semaphore close error\n");
		exit(ERROR);
	};
    return 0;
}

int init_variables() {
	if ((steps = mmap(NULL, sizeof *steps, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		handle_error("variable init error");
	if ((adultsInCenter = mmap(NULL, sizeof *adultsInCenter, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		handle_error("variable init error");
	if ((childrenInCenter = mmap(NULL, sizeof *childrenInCenter, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		handle_error("variable init error");
	if ((ended_p = mmap(NULL, sizeof *ended_p, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		handle_error("variable init error");
	if ((adultsCreated = mmap(NULL, sizeof *adultsCreated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0))==MAP_FAILED)
		handle_error("variable init error");
    if ((waiting = mmap(NULL, sizeof *waiting, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0))==MAP_FAILED)
    	handle_error("variable init error");
    if ((leaving = mmap(NULL, sizeof *leaving, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0))==MAP_FAILED)
    	handle_error("variable init error");
	return 0;
}

int deinit_variables() {
	if (munmap(steps, sizeof *steps)==-1) {
		fprintf(stderr, "variable deinit fail\n");
		exit(ERROR);
	};
	if (munmap(adultsInCenter, sizeof *adultsInCenter)==-1) {
		fprintf(stderr, "variable deinit fail\n");
		exit(ERROR);
	};
	if (munmap(childrenInCenter, sizeof *childrenInCenter)==-1){
		fprintf(stderr, "variable deinit fail\n");
		exit(ERROR);
	};
	if (munmap(ended_p, sizeof *ended_p)==-1) {
		fprintf(stderr, "variable deinit fail\n");
		exit(ERROR);
	};
	if (munmap(adultsCreated, sizeof *adultsCreated)==-1) {
		fprintf(stderr, "variable deinit fail\n");
		exit(ERROR);
	};   
    if (munmap(waiting, sizeof *waiting)==-1) {
		fprintf(stderr, "variable deinit fail\n");
		exit(ERROR);
	};
    if (munmap(leaving, sizeof *leaving)==-1) {
		fprintf(stderr, "variable deinit fail\n");
		exit(ERROR);
	};
	return 0;
}

void handle_error(char *message) {
	fprintf(stderr, "%s\n", message);
	deinit_variables();
	close_semaphores();
	exit(2);
}

int adult_process(int i) {
    //NEW ADULT
    sem_wait(mutex);
        *adultsCreated = *adultsCreated+1;
        fprintf(output, "%d\t: A %d\t: started\n", *steps, i+1);
        *steps = *steps+1;
    sem_post(mutex);
    //ENTER    
    sem_wait(mutex);
        fprintf(output, "%d\t: A %d\t: enter\n", *steps, i+1);
        *steps = *steps+1;
        *adultsInCenter = *adultsInCenter+1;
        //LBOS
        if (*waiting) {
            int n;
            if (*waiting<3) n = *waiting; else n = 3;
            for (int i = 0; i<n; i++) {
                sem_post(childQueue);
                sem_wait(childResponse);
            }
            *waiting -= n; 
            *childrenInCenter = *childrenInCenter+n;
        }
    sem_post(mutex);                                   
    //SLEEP - ACTIVITY IN CENTER
    usleep(1000*(rand()%(AWT+1)));
    //LEAVING CENTER - TRYING
    sem_wait(mutex);
        fprintf(output, "%d\t: A %d\t: trying to leave\n", *steps, i+1);
        *steps = *steps+1;    
    //WAITING
    //LBOS
    if (*childrenInCenter<=(*adultsInCenter-1)*3) {
        *adultsInCenter=*adultsInCenter-1;
        fprintf(output,"%d\t: A %d\t: leave\n", *steps, i+1);
        *steps = *steps+1;
        sem_post(mutex);
        if (*adultsInCenter == 0 && *adultsCreated == A) {
    		for (int i = 0; i<*waiting; i++) {
    			sem_post(childQueue);
    			sem_wait(childResponse);
    		}
    	} 
    }
    else{
        *leaving  = *leaving+1;
        fprintf(output, "%d\t: A %d\t: waiting : %d : %d\n", *steps, i+1, *adultsInCenter, *childrenInCenter);
        *steps = *steps+1;
        sem_post(mutex);
        sem_wait(adultQueue);
        fprintf(output,"%d\t: A %d\t: leave\n", *steps, i+1);
        *steps = *steps+1;
        if (*adultsInCenter == 0 && *adultsCreated == A) {
    		for (int i = 0; i<*waiting; i++) {
    			sem_post(childQueue);
    			sem_wait(childResponse);
    		}
    	}  
        sem_post(parentResponse);
    }
    //LEAVING CENTER         
    sem_wait(mutex_multi);        
    *ended_p += 1;
    sem_post(mutex_multi);
    
    //WAIT FOR ALL TO FINISH
    if (*ended_p == (A+C)) {
        sem_post(barrier);
    }
    sem_wait(barrier);
    sem_post(barrier);

    //FINISHING
    sem_wait(mutex);
        fprintf(output, "%d\t: A %d\t: finished\n", *steps, i+1);
        *steps = *steps+1;
    sem_post(mutex);

    exit(0);
}

int child_process(int i) {    
    //NEW CHILD, ENTERING
    sem_wait(mutex);
        fprintf(output, "%d\t: C %d\t: started\n", *steps, i+1);
        *steps = *steps+1;                    
    //WAITING
    //LBOS
        if (*childrenInCenter<3*(*adultsInCenter) || (*adultsCreated==A && *adultsInCenter==0)) {
            *childrenInCenter = *childrenInCenter+1;
            fprintf(output, "%d\t: C %d\t: enter\n", *steps, i+1);
            *steps = *steps+1;
            sem_post(mutex);
        }
        else {
            *waiting = *waiting+1;
            fprintf(output, "%d\t: C %d\t: waiting : %d : %d\n", *steps, i+1, *adultsInCenter, *childrenInCenter);
            *steps = *steps+1;
            sem_post(mutex);
            sem_wait(childQueue);
            fprintf(output, "%d\t: C %d\t: enter\n", *steps, i+1);
            *steps = *steps+1;
            sem_post(childResponse);
        }
    //WAITING IN CENTER
    usleep(1000*(rand()%(CWT+1)));
    //LEAVING CENTER - TRYING
    sem_wait(mutex);
        fprintf(output, "%d\t: C %d\t: trying to leave\n", *steps, i+1);
        *steps = *steps+1;
    //LEAVING
        fprintf(output, "%d\t: C %d\t: leave\n", *steps, i+1);
        *steps = *steps+1;
        *childrenInCenter = *childrenInCenter-1;
        //LBOS
        if (*leaving && (*childrenInCenter<=(*adultsInCenter-1)*3)) {
                *leaving = *leaving-1;
                *adultsInCenter = *adultsInCenter-1;
                sem_post(adultQueue);
                sem_wait(parentResponse);
        }
    sem_post(mutex);
    sem_wait(mutex_multi);
    *ended_p += 1;
    sem_post(mutex_multi);
    //WAIT FOR ALL TO FINISH
    if (*ended_p == (A+C)) {
        sem_post(barrier);
    }
    sem_wait(barrier);
    sem_post(barrier);

    //FINISHING
    sem_wait(mutex);
        fprintf(output, "%d\t: C %d\t: finished\n", *steps, i+1);
        *steps = *steps+1;
    sem_post(mutex);
    exit(0);
}