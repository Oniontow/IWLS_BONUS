# Project: mps

CPP      = g++
CFLAGS 	 = -c 
RM       = rm.exe -f
OPTFLAGS = -O2
TARGET = src/main.cpp src/libabc.a

bin/main: bin/main.o
	$(CPP) -O2 bin/main.o src/libabc.a -lm -ldl -lreadline -lpthread -o bin/main

bin/main.o:src/main.cpp
	$(CPP) -O2 -c src/main.cpp -o bin/main.o

clean:
		rm -rf *.o bin/*