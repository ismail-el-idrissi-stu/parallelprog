CC=gcc
CFLAGS=-g -O3 -fstrict-aliasing -ftree-vectorize -fopenmp-simd \
 -march=native -mtune=native -mprefer-vector-width=256 \
 -fopt-info-vec-optimized -fopt-info-vec-missed

stream_triad: main.o timestep.o timer.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm -rf ./stream_triad ./stream_triad.o ./timer.o ./main.o ./timestep.o