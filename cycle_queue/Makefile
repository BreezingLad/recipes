
all:
	@echo -e "make sem_perf"
	@echo -e "make cond_perf"
sem_perf_obj=./sem/CycleQueue.o testPerformance.o
cond_perf_obj=./cond/CycleQueue.o testPerformance.o

sem_perf: $(sem_perf_obj)
	g++ -O3 -o sem_perf $(sem_perf_obj) -lpthread

cond_perf: $(cond_perf_obj)
	g++ -O3 -o cond_perf $(cond_perf_obj) -lpthread 

%.o:%.cpp
	g++ -c $< -o $@
clean:
	rm -f *.o perf
