#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "employee.h"

bool
employee_key_match(void *data, void *key){
    /* 'data' was (void *) &employee */
    employee *e = (employee *) data;

    if (e->id == (uintptr_t) key)
	return true;
    else
	return false;
}

employee*
employee_generate(uintptr_t id, char *name){
    employee *e;

    if ((e = (employee *) malloc(sizeof(employee))) == NULL){
	perror("malloc");
	exit(-1);
    }

    e->id = id;
    strncpy(e->name, name, BUF_SIZE);
    e->rc.ref_count = 0;
    pthread_mutex_init(&e->rc.mutex, NULL);
    e->updated_flag = false;
    pthread_rwlock_init(&e->employee_rwlock, NULL);

    return e;
}
