#ifndef __EMPLOYEE__
#define __EMPLOYEE__

#include "Linked-List/linked_list.h"
#include "ref_count.h"

#define BUF_SIZE 64

typedef struct employee {

    uintptr_t id;
    char name[BUF_SIZE];
    bool updated_flag;
    ref_count rc;

    /* Container-element level read/write lock */
    pthread_rwlock_t employee_rwlock;

} employee;

typedef struct employees_list {

    linked_list *elist;

    /* Container level read/write lock */
    pthread_rwlock_t elist_rwlock;

} employees_list;

bool employee_key_match(void *data, void *key);
employee* employee_generate(uintptr_t id, char *name);

#endif
