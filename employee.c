#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "employee.h"
#include "Read-Write-Locks/rw_locks.h"

bool
employee_key_match(void *data, void *key){
    /* Expect 'data' is (void *) &employee */
    employee *e = (employee *) data;

    if (e->id == (uintptr_t) key)
	return true;
    else
	return false;
}

employee*
employee_generate(uintptr_t id, char *name){
#define MAX_THREADS_ACCESS 4

    employee *e;

    if ((e = (employee *) malloc(sizeof(employee))) == NULL){
	perror("malloc");
	exit(-1);
    }

    /* basic info */
    e->id = id;
    strncpy(e->name, name, BUF_SIZE);
    e->updated_flag = false;

    /* init ref_count */
    e->rc.ref_count = 0;
    pthread_mutex_init(&e->rc.mutex, NULL);

    /* init rw_lock */
    e->employee_rwlock = rw_lock_init(MAX_THREADS_ACCESS);

    return e;
}
