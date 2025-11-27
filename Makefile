default: PartialSum

all: PartialSum

%.o: %.c
	mpicc -c $< -o $@

PartialSum: PartialSum.o timer.o
	mpicc PartialSum.o timer.o -o PartialSum

run:
	mpirun -np 4 ./PartialSum

clean:
	rm -f PartialSum *.o
