CC=gcc
CFLAGS=-O3 -march=native
LDFLAGS=-lm
BIN_DIR=../bin

all: $(BIN_DIR)/neighbors2d $(BIN_DIR)/sum_bench

$(BIN_DIR)/neighbors2d: neighbors2d_hashcpu.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(BIN_DIR)/sum_bench: sum_bench.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(BIN_DIR)/neighbors2d $(BIN_DIR)/sum_bench