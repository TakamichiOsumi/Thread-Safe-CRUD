#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ref_count.h"
#include "employee.h"
#include "employee_crud.h"
#include "Linked-List/linked_list.h"
#include "Read-Write-Locks/rw_locks.h"

#define THREADS_LOOP 4000
/*
 * Narrow down the range of the ids where threads access,
 * which results in their simultaneous accesses to the same data for debugging.
 */
#define EMPLOYEE_ID_RANGE 8
#define MAX_THREADS_ACCESS 4


/* Define some utility functions for debugging */
void
assert_dump_handler(int sig, siginfo_t *info, void *q){
    fprintf(stderr,
	    "\n"
	    "!!! the thread id where raised assertion failure : %p\n"
	    "\n",
	    pthread_self());
    exit(-1);
}

void
prepare_assertion_failure(void){
    struct sigaction act;

    act.sa_sigaction = assert_dump_handler;
    act.sa_flags = SIGABRT;
    sigemptyset(&act.sa_mask);

    sigaction(SIGABRT, &act, NULL);
}

/* thread for CREATE operation */
static void *
create_thread(void *arg){
    employees_list *elist = (employees_list *) arg;
    char buf[BUF_SIZE];
    int i;
    uintptr_t c;
    
    for (i = 0; i < THREADS_LOOP; i++){
	c = i % EMPLOYEE_ID_RANGE;
	snprintf(buf, BUF_SIZE, "%lu", c);
	printf("debug : %s (%p) will register an employee whose id = %lu\n",
	       __FUNCTION__, pthread_self(), c);
	emp_create_op_fn(elist, c, buf);
    }

    return NULL;
}

/* thread for READ operation */
static void *
read_thread(void *arg){
    employees_list *elist = (employees_list *) arg;
    int i;
    uintptr_t c;

    for (i = 0; i < THREADS_LOOP; i++){
	c = i % EMPLOYEE_ID_RANGE;
	printf("debug : %s (%p) will read an employee whose id = %lu\n",
	       __FUNCTION__, pthread_self(), c);
	emp_read_op_fn(elist, (void *) c);
    }

    return NULL;
}

/* thread for UPDATE operation */
static void *
update_thread(void *arg){
    employees_list *elist = (employees_list *) arg;
    int i;
    uintptr_t c;

    for (i = 0; i < THREADS_LOOP; i++){
	c = i % EMPLOYEE_ID_RANGE;
	printf("debug : %s (%p) will update an employee whose id = %lu\n",
	       __FUNCTION__, pthread_self(), c);
	emp_update_op_fn(elist, (void *) c);
    }

    return NULL;
}

/* thread for DELETE operation */
static void *
delete_thread(void *arg){
    employees_list *elist = (employees_list *) arg;
    int i;
    uintptr_t c;

    for (i = 0; i < THREADS_LOOP; i++){
	c = i % EMPLOYEE_ID_RANGE;
	printf("debug : %s (%p) will delete an employee whose id = %lu\n",
	       __FUNCTION__, pthread_self(), c);
	emp_delete_op_fn(elist, (void *) c);
    }

    return NULL;
}

int
main(void){
    pthread_t thread_handlers[4];
    employees_list *employees_container;

    prepare_assertion_failure();

    if ((employees_container = (employees_list *) malloc(sizeof(employees_list))) == NULL){
	perror("malloc");
	exit(-1);
    }
    
    employees_container->elist = ll_init(employee_key_match);
    employees_container->elist_rwlock = rw_lock_init(MAX_THREADS_ACCESS);

    pthread_create(&thread_handlers[0], NULL, create_thread,
		   employees_container);
    pthread_create(&thread_handlers[1], NULL, read_thread,
		   employees_container);
    pthread_create(&thread_handlers[2], NULL, update_thread,
		   employees_container);
    pthread_create(&thread_handlers[3], NULL, delete_thread,
		   employees_container);

    pthread_exit(0);

    return 0;
}
