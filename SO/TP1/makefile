#	Name	of	the	project
PROJ_NAME=./tp1
	
#	.c	files
CPP_SOURCE=$(wildcard	*.cpp)	$(wildcard	*/*.cpp)	
#	.h	files
H_SOURCE=$(wildcard	*.h)	$(wildcard	*/*.h)
	
#	Object	files
OBJ=$(CPP_SOURCE:.cpp=.o)
	
LIBS= -pthread
#	Compiler
CC=g++
	
#	Flags	for	compiler
CC_FLAGS= -c -std=c++11 -pthread
	
#
#	Compilation	and	linking
#
all:	$(PROJ_NAME)
	

$(PROJ_NAME):	$(OBJ)
	$(CC)	-o	$@	$^ $(LIBS)
	
%.o:	%.cpp	%.h
	
	$(CC)	-o	$@	$<	$(CC_FLAGS) $(LIBS)
	
main.o:	main.cpp	$(H_SOURCE)
	$(CC)	-o	$@	$<	$(CC_FLAGS) $(LIBS)
	
clean:
	rm	-rf	*.o	$(PROJ_NAME)	*~	
