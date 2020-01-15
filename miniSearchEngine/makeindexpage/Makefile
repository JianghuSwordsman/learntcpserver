INC_DIR := ./inc
SRC_DIR := ./src
EXE_DIR := ./bin
TARGET := $(EXE_DIR)/main
CPPFLAGS := -g -o 
CC := g++
OBJS_CC :=$(wildcard  $(SRC_DIR)/*.cc)
OBJS_H :=$(wildcard  $(INC_DIR)/*.h)
$(TARGET):$(OBJS_CC) $(OBJS_H)
	$(CC) $(CPPFLAGS) $@ $(OBJS_CC) -DOS_LINUX -I$(INC_DIR) -L./lib -lNLPIR -ljson -Wl,-rpath=../lib -Wno-deprecated -std=c++11
clean:
	rm -rf $(TARGET)
