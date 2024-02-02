CC	= gcc
CFLAGS	= -O0 -Wall
SUBDIR	= Linked-List
LIB_PATH	= -L $(CURDIR)/$(SUBDIR)
LOCAL_LIBS	= -llinked_list
SUBMODULE_IS_READY	= ./$(SUBDIR)/Makefile
PROGRAM	= exec_crud_threads

all: $(PROGRAM)

liblinked_list.a:
ifeq ($(SUBMODULE_IS_READY), $(wildcard ./$(SUBDIR)/Make*))
	@cd $(SUBDIR); make; cd ../
else
	$(error $(SUBDIRS)/Makefile is missing. Run 'git submodule init&update')
endif

$(PROGRAM): liblinked_list.a employee_crud.o ref_count.o
	$(CC) $(CFLAGS) $(LIB_PATH) $(LOCAL_LIBS)  launch_employee_CRUD_threads.c employee_crud.o ref_count.o employee.o -o $(PROGRAM)

ref_count.o: ref_count.c
	$(CC) $(CFLAGS) ref_count.c -c

employee_crud.o: employee.o
	$(CC) $(CFLAGS) employee_crud.c -c

employee.o: employee.c
	$(CC) $(CFLAGS) employee.c -c

.phony: clean test

clean:
ifeq ($(SUBMODULE_IS_READY), $(wildcard ./$(SUBDIR)/Make*))
	@cd $(SUBDIR); make clean; cd ../
	rm -rf $(PROGRAM) *.o
else
	rm -rf $(PROGRAM) *.o
endif

test: $(PROGRAM)
	@./$(PROGRAM) && echo "Successful if the result is zero >>> $$?"
