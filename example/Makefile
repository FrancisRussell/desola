include /home/fpr02/individual_project/taskgraph/Makefile.conf
include /home/fpr02/individual_project/taskgraph/Makefile.defs
CCFLAGS+=$(TASKINCLUDEPATHS) -I..
LDFLAGS+=$(TASKLIBPATHS) $(TASKLIBS)

test: test.o
	$(CC) $^ $(LDFLAGS)  -o $@

test.o:
	$(CC) $(CCFLAGS) -I../include -c test.cc -o test.o

clean:
	rm -f test.o test
