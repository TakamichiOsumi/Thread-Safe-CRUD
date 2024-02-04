#ifndef __EMPLOYEE_CRUD__
#define __EMPLOYEE_CRUD__

#include "employee.h"

typedef enum { EMP_CREATE, EMP_READ, EMP_UPDATE, EMP_DELETE } emp_crud_op;

char *emp_crud_string(emp_crud_op);
void emp_create_op_fn(employees_list *list, uintptr_t id, char *name);
void emp_read_op_fn(employees_list *list, void *key);
void emp_update_op_fn(employees_list *list, void *key);
void emp_delete_op_fn(employees_list *list, void *key);

#endif
