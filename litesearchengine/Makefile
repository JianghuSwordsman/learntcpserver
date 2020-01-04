INC_DIR:=./inc/ ./inc/net/ ./inc/threadpool/
SRC_DIR:=./src/ ./src/net/ ./src/threadpool/
SRCS:=$(wildcard ./src/*.cc) $(wildcard ./src/net/*.cc) $(wildcard ./src/threadpool/*.cc)
LIBS:= -lpthread

CXX:=g++
CXXFLAGS:= -Wall -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/main

$(EXE):$(SRCS)
	$(CXX) -o $(EXE) $(SRCS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBLS)
