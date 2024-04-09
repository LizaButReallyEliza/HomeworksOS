CXX = g++
LIB_DIR = $(PWD)

all: parallel_scheduler.so demo-application

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	$(CXX) parallel_scheduler.cpp -c -fPIC

parallel_scheduler.so: parallel_scheduler.o
	$(CXX) -shared parallel_scheduler.o -o libparallel_scheduler.so

demo-application: demo-application.cpp parallel_scheduler.h parallel_scheduler.so
	$(CXX) demo-application.cpp -o demo-application -lparallel_scheduler -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR)

clean:
	rm -f *.o *.so demo-application

