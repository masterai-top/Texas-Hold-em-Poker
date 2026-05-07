BIN_EXE=a.out
BIN_SRC=ShareMem.cpp HandIndex.cpp Deck.cpp Main.cpp
BIN_OBJ=$(patsubst %.cpp, %.o, $(BIN_SRC))
#
LIB_EXE=libHandClus.a
LIB_SRC=ShareMem.cpp HandIndex.cpp Deck.cpp
LIB_OBJ=$(patsubst %.cpp, %.o, $(LIB_SRC))
#
CPP = g++
INCLUDE = -I./ -I/usr/local/include/NumCpp/ -L./
LIBRARY = -lboost_iostreams -lboost_serialization -llog4cplus
CPPFLAGS += -std=c++17 -pthread -O0 -g
#
AR = ar
ARFLAGS = rcs 
###########################################
all: $(BIN_EXE) $(LIB_EXE)
	@echo "compiling..."
###########################################
$(BIN_EXE): $(BIN_OBJ)
	$(CPP) $(CPPFLAGS) $(INCLUDE) $(LIBRARY) -o $(BIN_EXE) $(BIN_OBJ)
###########################################
$(LIB_EXE): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $(LIB_EXE) $(LIB_OBJ)
###########################################
%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $(INCLUDE) $<
############################################
.PHONY:clean
clean:
	rm -f $(BIN_OBJ) $(BIN_EXE) $(LIB_EXE)
