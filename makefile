 #Program Name:
PROG = TSP

CC = g++

SRC = TSP.cpp 
INC= TSP.h
OBJS= TSP.o


all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(OBJS) $(INC) -o $(PROG) 

$(OBJS): $(SRC) 
	$(CC)   -c $(SRC)


clean: 
	rm -f $(PROG) $(OBJS)

