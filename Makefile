all: main

main: main.cpp
	g++ -Wall -Wconversion -o main.exe main.cpp Display\display.cpp Scheduler\scheduler.cpp -ld2d1 -lgdi32 -luser32 -lm