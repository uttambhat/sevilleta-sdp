CC = g++
CFLAGS = -O2 -std=c++11 -fopenmp

INC_DIR = include
OBJ_DIR = build
SRC_DIR = src
BIN_DIR = bin
DAT_DIR = data

all: $(BIN_DIR)/main.out $(BIN_DIR)/ksim.out

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.C $(INC_DIR)/sdp.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJ_DIR)/sdp.o: $(SRC_DIR)/sdp.C $(INC_DIR)/sdp.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)/ksim.o: $(SRC_DIR)/ksim.C $(INC_DIR)/parameters.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(BIN_DIR)/main.out: $(OBJ_DIR)/main.o $(OBJ_DIR)/sdp.o
	$(CC) -o $@ $^ $(CFLAGS)
	
$(BIN_DIR)/ksim.out: $(OBJ_DIR)/ksim.o 
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ_DIR)/*.o
	
dataclean:
	rm $(DAT_DIR)/decision/* $(DAT_DIR)/fitness/*

.PHONY: clean dataclean



