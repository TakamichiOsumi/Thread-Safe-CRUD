CC	= gcc
CFLAGS	= -O0 -Wall
SUBDIR1	= Linked-List
SUBDIR2 = Read-Write-Locks
SUBDIRS	= $(SUBDIR1) $(SUBDIR2)
LIB_PATH	= -L $(CURDIR)/$(SUBDIR1) -L $(CURDIR)/$(SUBDIR2)
LOCAL_LIBS	= -llinked_list -lrw_lock
PROGRAM	= exec_crud_threads

all: $(PROGRAM)

liblinked_list.a librw_lock.a:
	for dir in $(SUBDIRS); do make -C $$dir; done

$(PROGRAM): liblinked_list.a librw_lock.a employee_crud.o ref_count.o
	$(CC) $(CFLAGS) $(LIB_PATH) $(LOCAL_LIBS) launch_employee_CRUD_threads.c employee_crud.o ref_count.o employee.o -o $(PROGRAM)

ref_count.o: ref_count.c
	$(CC) $(CFLAGS) ref_count.c -c

employee_crud.o: employee.o
	$(CC) $(CFLAGS) employee_crud.c -c

employee.o: employee.c
	$(CC) $(CFLAGS) employee.c -c

.phony: clean test

clean:
	for dir in $(SUBDIRS); do cd $$dir; make clean; cd ..; done
	rm -rf $(PROGRAM) *.o

test: $(PROGRAM)
	@./$(PROGRAM) &> /dev/null && echo "Successful if the result is zero >>> $$?"
