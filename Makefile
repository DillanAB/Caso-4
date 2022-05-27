all: gen

gen:
	g++ main.cpp -o prog.exe -lws2_32