#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Read-Write-Locks/rw_locks.h"
#include "ref_count.h"
#include "employee_crud.h"
#include "employee.h"

char *
emp_crud_string(emp_crud_op op){
    switch(op){
	case EMP_CREATE:
	    return "CREATE";
	case EMP_READ:
	    return "READ";
	case EMP_UPDATE:
	    return "UPDATE";
	case EMP_DELETE:
	    return "DELETE";
	default:
	    assert(0);
	    return NULL;
    }
}

static void
emp_crud_succeed(emp_crud_op op, employee *e){
    char *string_op = emp_crud_string(op);

    printf("[%s] OK : employee with id = '%lu', name = '%s' and updated == '%s'\n",
	   string_op, e->id, e->name, e->updated_flag == true ? "yes" : "no");
}

/*
 * Don't take an argument of 'employee' type.
 *
 * It's because in failure case, it's possible the employee data isn't ready
 * or is incomplete.
 */
static void
emp_crud_fail(emp_crud_op op, uintptr_t id, char *name, bool updated){
    char *string_op = emp_crud_string(op);

    switch(op){
	case EMP_CREATE:
	    printf("[%s] has failed because of duplicate element (id = %lu)\n",
		   string_op, id);
	    break;
	case EMP_READ:
	case EMP_UPDATE:
	case EMP_DELETE:
	    printf("[%s] has failed because of missing element (id = %lu)\n",
		   string_op, id);
	    break;
	default:
	    assert(0);
    }
}

static void
emp_crud_done(emp_crud_op op){
    char *string_op = emp_crud_string(op);

    printf("%s", string_op);
}

/* CREATE */
void
emp_create_op_fn(employees_list *list, uintptr_t id, char *name){
    node *n;
    uintptr_t key = id;
    employee *e;

    rw_lock_wr_lock(list->elist_rwlock);

    if ((n = ll_search_by_key(list->elist, (void *) key)) != NULL){
	emp_crud_fail(EMP_CREATE, key, name, false);
	rw_lock_unlock(list->elist_rwlock);
	return;
    }

    /* Perform the CREATE operation of the employee object */
    e = employee_generate(id, name);
    ll_insert(list->elist, e);
    emp_crud_succeed(EMP_CREATE, e);
    ref_count_inc(&e->rc);

    rw_lock_unlock(list->elist_rwlock);
}

/* READ */
void
emp_read_op_fn(employees_list *list, void *key){
    employee *e;
    node *n;

    rw_lock_rd_lock(list->elist_rwlock);

    if ((n = ll_search_by_key(list->elist, key)) == NULL){
	emp_crud_fail(EMP_READ, (uintptr_t) key, NULL, false);
	rw_lock_unlock(list->elist_rwlock);
	return;
    }

    e = (employee *) n->data;
    thread_using_object(&e->rc);
    rw_lock_rd_lock(e->employee_rwlock);
    rw_lock_unlock(list->elist_rwlock);

    /* Perform the UPDATE operation on the employee object */
    emp_crud_succeed(EMP_READ, e);

    rw_lock_unlock(e->employee_rwlock);

    if (thread_using_object_done(&e->rc)){
	printf("READ operation has done for employee id = %lu\n",
	       (uintptr_t) e->id);
    }
}

/* UPDATE */
void
emp_update_op_fn(employees_list *list, void *key){
    employee *e;
    node *n;

    rw_lock_rd_lock(list->elist_rwlock);

    if ((n = ll_search_by_key(list->elist, key)) == NULL){
	emp_crud_fail(EMP_UPDATE, (uintptr_t) key, NULL, false);
	rw_lock_unlock(list->elist_rwlock);
	return;
    }

    e = (employee *) n->data;
    thread_using_object(&e->rc);
    rw_lock_wr_lock(e->employee_rwlock);
    rw_lock_unlock(list->elist_rwlock);

    /* Perform the UPDATE operation on the employee object */
    e->updated_flag = true;
    emp_crud_succeed(EMP_UPDATE, e);

    rw_lock_unlock(e->employee_rwlock);

    if (thread_using_object_done(&e->rc)){
	printf("UPDATE operation has done for employee id = %lu\n",
	       (uintptr_t) e->id);
    }
}

/* DELETE */
void
emp_delete_op_fn(employees_list *list, void *key){
    employee *e;
    node *n;

    rw_lock_wr_lock(list->elist_rwlock);

    if ((n = ll_search_by_key(list->elist, key)) == NULL){
	emp_crud_fail(EMP_DELETE, (uintptr_t) key, NULL, false);
	rw_lock_unlock(list->elist_rwlock);
	return;
    }

    /* Perform the UPDATE operation on the employee object */
    e = (employee *) n->data;
    thread_using_object(&e->rc);
    n = ll_remove(list->elist, key);
    emp_crud_succeed(EMP_DELETE, e);

    rw_lock_unlock(list->elist_rwlock);

    assert(ref_count_dec_and_iszero(&e->rc) == false);

    if (thread_using_object_done(&e->rc)){
	free(e);
    }else{
	printf("DELETE employee whose id = %lu will be deferred\n",
	       (uintptr_t) e->id);
    }
}
